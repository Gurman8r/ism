#include <core/api/object/base_object.hpp>
#include <servers/script_server.hpp>

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

void Object::_construct_object()
{
	static InstanceID id{};
	m_instance_id = ++id;
	m_refcount.init();
	m_refcount_init.init();
}

Object::Object(TYPE const & type) noexcept : m_type{ type } { _construct_object(); }

Object::~Object() { m_type = nullptr; m_instance_id = 0; }

TYPE Object::get_class() noexcept { return &_class_type_static; }

TYPE Object::get_type() const noexcept { if (!m_type) { m_type = _get_typev(); } return m_type; }

bool Object::set_type(TYPE const & value) noexcept { return (m_type = value).is_valid(); }

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

ISM_CLASS_IMPLEMENTATION(Object, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattro = (getattrofunc)generic_getattr;

	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)object_alloc;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<void *>{}(self.ptr()); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Object::_bind_class(OBJ scope)
{
	CLASS_<OBJ>(scope, "object")

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::object_alloc(TYPE type)
{
	return (Object *)memalloc(type->tp_size);
}

OBJ ism::object_new(TYPE type, OBJ args)
{
	if (type.has_feature(TypeFlags_IsAbstract)) { return nullptr; }

	return type->tp_alloc(type);
}

Error ism::object_init(OBJ self, OBJ args)
{
	TYPE type{ CHECK(typeof(self)) };

	Error err{ Error_None };

	return err;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ CHECK(typeof(obj)) };

	if (!type->tp_dict && !type->ready()) { return nullptr; }

	OBJ descr{ type.lookup(name) };

	descrgetfunc fn{};

	if (TYPE dtype; descr && (dtype = typeof(descr)))
	{
		fn = dtype->tp_descr_get;

		if (fn && dtype->tp_descr_set)
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
	TYPE type{ CHECK(typeof(obj)) };

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