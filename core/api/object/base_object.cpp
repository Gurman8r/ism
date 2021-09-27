#include <core/api/object/base_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		api::get_internals().add_class(&ob_class_type);

		VALIDATE(ob_class_type.tp_bind)(&ob_class_type);
	}
}

void Object::_initialize_classv() { initialize_class(); }

Object::Object() noexcept { m_refcount.init(); m_refcount_init.init(); }

Object::~Object() { m_type = nullptr; }

bool Object::init_ref()
{
	if (reference())
	{
		if (!has_references() && m_refcount_init.unref())
		{
			unreference(); // first referencing is already 1, so compensate for the ref above
		}
		return true;
	}
	return false;
}

bool Object::reference()
{
	uint32_t rc_val{ m_refcount.refval() };
	bool success{ rc_val != 0 };
	if (success && rc_val <= 2 /* higher is not relevant */) { on_reference(); }
	return success;
}

bool Object::unreference()
{
	uint32_t rc_val{ m_refcount.unrefval() };
	bool die{ rc_val == 0 };
	if (rc_val <= 1 /* higher is not relevant */) { on_unreference(); }
	return die;
}

TYPE Object::get_type_static() noexcept { return &ob_class_type; }

TYPE Object::_get_typev() const { return get_type_static(); }

TYPE Object::get_type() const noexcept { return ((!!m_type) || (m_type = _get_typev())), m_type; }

void Object::set_type(TYPE const & value) noexcept { m_type = value; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(Object, t, TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_getattro = (getattrofunc)api::generic_getattr;

	t.tp_setattro = (setattrofunc)api::generic_setattr;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(self.ptr()); };

	t.tp_bind = BIND_CLASS_HELPER(Object, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::api::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ api::typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJ descr{ type.lookup(name) };

	descrgetfunc get{};

	if (descr)
	{
		TYPE descr_type{ api::typeof(descr) };

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

Error ism::api::generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict)
{
	TYPE type{ api::typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return Error_Unknown; }

	OBJ descr{ type.lookup(name) };

	descrsetfunc set{};

	if (descr)
	{
		set = api::typeof(descr)->tp_descr_set;

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