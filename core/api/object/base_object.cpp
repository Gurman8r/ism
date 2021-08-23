#include <core/api/object/base_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		get_internals().add_class(&_class_type_static);

		_class_type_static.tp_bind(&_class_type_static);
	}
}

void Object::_initialize_classv() { initialize_class(); }

void Object::_construct_object() { m_refcount.init(); m_refcount_init.init(); }

Object::Object(TYPE const & type) noexcept : ob_type{ type } { _construct_object(); }

Object::~Object() { ob_type = nullptr; }

TYPE Object::get_class() noexcept { return &_class_type_static; }

TYPE Object::_get_typev() const { return get_class(); }

TYPE Object::get_type() const noexcept { if (!ob_type) { ob_type = _get_typev(); } return ob_type; }

bool Object::set_type(TYPE const & value) noexcept { return (ob_type = value).is_valid(); }

bool Object::init_ref()
{
	if (inc_ref())
	{
		if (!has_references() && m_refcount_init.unref())
		{
			dec_ref(); // first referencing is already 1, so compensate for the ref above
		}
		return true;
	}
	return false;
}

bool Object::inc_ref()
{
	uint32_t rc_val{ m_refcount.refval() };
	bool success{ rc_val != 0 };
	if (success && rc_val <= 2 /* higher is not relevant */) { on_inc_ref(); }
	return success;
}

bool Object::dec_ref()
{
	uint32_t rc_val{ m_refcount.unrefval() };
	bool die{ rc_val == 0 };
	if (rc_val <= 1 /* higher is not relevant */) { on_dec_ref(); }
	return die;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPLEMENTATION(Object, t, "object", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(self.ptr()); };

	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<OBJ>(type);
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

	if (descr && (set = typeof(descr)->tp_descr_set))
	{
		return set(descr, obj, value);
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