#include <core/object/base_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		get_internals().add_class(&ob_class_type);

		CHECK(ob_class_type.tp_bind)(&ob_class_type);
	}
}

void Object::_initialize_classv() { initialize_class(); }

Object::Object() noexcept { m_refcount.init(); m_refcount_init.init(); }

Object::~Object() { m_type = nullptr; }

TYPE Object::get_type_static() noexcept { return &ob_class_type; }

TYPE Object::_get_typev() const { return get_type_static(); }

TYPE Object::get_type() const noexcept { if (!m_type) { m_type = _get_typev(); } return m_type; }

bool Object::set_type(TYPE const & value) noexcept { return (m_type = value).is_valid(); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(Object, t, TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(self.ptr()); };

	t.tp_bind = CLASS_BINDER(Object, c)
	{
		return c;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJ descr{ type.lookup(name) };

	descrgetfunc get{};

	if (descr)
	{
		TYPE descr_type{ typeof(descr) };

		get = descr_type->tp_descr_get;

		if (get && descr_type->tp_descr_set)
		{
			return get(descr, obj, type);
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

	if (get) { return get(descr, obj, type); }

	if (descr) { return descr; }

	return nullptr;
}

Error ism::generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return Error_Unknown; }

	OBJ descr{ type.lookup(name) };

	descrsetfunc set{};

	if (descr)
	{
		set = typeof(descr)->tp_descr_set;

		if (set)
		{
			return set(descr, obj, value);
		}
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