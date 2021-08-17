#include <core/api/object/base_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::initialize_class(OBJ scope)
{
	if (static bool once{}; !once && (once = true))
	{
		TypeDB::add_class<Object>();

		_bind_class(scope);
	}
}

void Object::_initialize_classv(OBJ scope) { initialize_class(scope); }

TYPE Object::_get_typev() const { return get_class(); }

Object::Object(TYPE const & type) noexcept : m_type{ type }
{
	static InstanceID id{};
	m_instance_id = ++id;
	m_refcount.init();
	m_refcount_init.init();
}

Object::~Object() { m_type = nullptr; }

TYPE Object::get_class() { return &_class_type_static; }

TYPE Object::get_type() const { if (!m_type) { m_type = _get_typev(); } return m_type; }

bool Object::set_type(TYPE const & value) { return (m_type = value).is_valid(); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(Object, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(*self); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return CMP(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::_bind_class(OBJ scope)
{
	CLASS_<OBJ>(scope, "object", get_class())

		//.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::object_alloc(TYPE type)
{
	return nullptr;
}

Error ism::object_init(OBJ self, OBJ args)
{
	return Error_None;
}

OBJ ism::object_new(TYPE type, OBJ args)
{
	return type->tp_new(type, args);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJ descr{ type.lookup(name) };

	descrgetfunc fn{};

	if (TYPE dtype; descr && (dtype = typeof(descr)))
	{
		if ((fn = dtype->tp_descr_get) && dtype->tp_descr_set)
		{
			return fn(descr, obj, type);
		}
	}

	if (OBJ * dictptr; !dict && (dictptr = get_dict_ptr(type, obj)))
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

Error ism::generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return Error_Unknown; }

	OBJ descr{ type.lookup(name) };

	descrsetfunc fn{};

	if (descr && (fn = typeof(descr)->tp_descr_set))
	{
		return fn(descr, obj, value);
	}

	if (!dict)
	{
		if (OBJ * dictptr{ get_dict_ptr(type, obj) })
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