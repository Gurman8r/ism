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

ISM_OBJECT_TYPE_STATIC(BaseObject, t)
{
	t.tp_name = "object";
	t.tp_size = sizeof(BaseObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = nullptr;

	t.tp_getattro = (getattrofunc)generic_getattr;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((BaseObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
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
	auto const size{ (size_t)type->tp_size };

	auto const obj{ (BaseObject *)memalloc(size) };

	return OBJECT(obj);
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
		if (OBJECT * result{ DICT(dict).lookup(name) })
		{
			return *result;
		}
		else
		{
			return nullptr;
		}
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
			if (!(dict = *dictptr)) { dict = DICT(DictObject{}); }

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