#include <core/api/object/base_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		Internals::add_class<Object>();

		_bind_methods();
	}
}

void Object::_initialize_classv() { initialize_class(); }

void Object::_construct_object() { m_refcount.init(); m_refcount_init.init(); }

Object::Object(TYPE const & type) noexcept : ob_type{ type } { _construct_object(); }

Object::~Object() { ob_type = nullptr; }

TYPE Object::get_class_static() noexcept { return &_class_type_static; }

TYPE Object::_get_typev() const { return get_class_static(); }

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

ISM_IMPLEMENT_CLASS_TYPE(Object, t)
{
	t.tp_name = "object";

	t.tp_size = sizeof(Object);

	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(self.ptr()); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };

	t.tp_del = (delfunc)[](Object * ptr) { memdelete(ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::_bind_methods()
{
	CLASS_<OBJ>()

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ typeof(obj) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJ descr{ type.lookup(name) };

	descrgetfunc fn{};

	if (descr)
	{
		TYPE descr_type{ typeof(descr) };

		fn = descr_type->tp_descr_get;

		if (fn && descr_type->tp_descr_set)
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