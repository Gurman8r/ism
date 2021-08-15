#include <core/api/object/base_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::initialize_class(OBJECT scope)
{
	if (static bool once{}; !once && (once = true))
	{
		TypeDB::add_class<BaseObject>();

		_bind_class(scope);
	}
}

void BaseObject::_initialize_classv(OBJECT scope) { initialize_class(scope); }

TYPE BaseObject::_get_typev() const { return get_type_static(); }

BaseObject::BaseObject(TYPE const & t) noexcept : ob_type{ *t } {}

BaseObject::~BaseObject() { ob_type = nullptr; }

TYPE BaseObject::get_type_static() { return &ob_type_static; }

TYPE BaseObject::get_type() const { if (!ob_type) { ob_type = _get_typev(); } return ob_type; }

bool BaseObject::set_type(TYPE const & value) { return (bool)(ob_type = value); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_COMPOSE_TYPE_OBJECT(BaseObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_hash = (hashfunc)[](OBJECT self) { return Hash<void *>{}(*self); };

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other) { return util::compare(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::_bind_class(OBJECT scope)
{
	CLASS_<OBJECT>(scope, "object", get_type_static())

		//.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::object_alloc(TYPE type)
{
	return nullptr;
}

Error ism::object_init(OBJECT self, OBJECT args)
{
	return Error_None;
}

OBJECT ism::object_new(TYPE type, OBJECT args)
{
	return type->tp_new(type, args);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::generic_getattr_with_dict(OBJECT obj, OBJECT name, OBJECT dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJECT descr{ type.lookup(name) };

	descrgetfunc fn{};

	if (TYPE dtype; descr && (dtype = typeof(descr)))
	{
		if ((fn = dtype->tp_descr_get) && dtype->tp_descr_set)
		{
			return fn(descr, obj, type);
		}
	}

	if (OBJECT * dictptr; !dict && (dictptr = get_dict_ptr(type, obj)))
	{
		dict = *dictptr;
	}

	if (dict)
	{
		return DICT(dict).lookup(name);
	}

	if (fn) { return fn(descr, obj, type); }

	if (descr) { return descr; }

	return nullptr;
}

Error ism::generic_setattr_with_dict(OBJECT obj, OBJECT name, OBJECT value, OBJECT dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return Error_Unknown; }

	OBJECT descr{ type.lookup(name) };

	descrsetfunc fn{};

	if (descr && (fn = typeof(descr)->tp_descr_set))
	{
		return fn(descr, obj, value);
	}

	if (!dict)
	{
		if (OBJECT * dictptr{ get_dict_ptr(type, obj) })
		{
			if (!(dict = *dictptr)) { dict = DICT::new_(); }

			return (DICT(dict)[name] = value), Error_None;
		}
		else
		{
			return Error_Unknown;
		}
	}
	else
	{
		return (DICT(dict)[name] = value), Error_None;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */