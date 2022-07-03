#include <core/object/base_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::initialize_class()
{
	static SCOPE_ENTER(&)
	{
		INTERNALS->add_class(&g_type_static);

		ASSERT(g_type_static.tp_install);

		ASSERT(g_type_static.tp_install(&g_type_static));
	};
}

void BaseObject::_initialize_classv() { initialize_class(); }

void BaseObject::_postinitialize() {}

bool BaseObject::_predelete() { return true; }

void BaseObject::_reference() {}

void BaseObject::_unreference() {}

BaseObject::BaseObject() noexcept { m_refcount.init(); m_refcount_init.init(); }

BaseObject::~BaseObject() { m_type = nullptr; }

bool BaseObject::init_ref()
{
	if (reference())
	{
		if (!has_references() && m_refcount_init.unref())
		{
			// first referencing is already 1, so compensate for the ref above
			unreference();
		}
		return true;
	}
	return false;
}

bool BaseObject::reference()
{
	uint32_t const rc_val{ m_refcount.refval() };
	bool const success{ rc_val != 0 };
	if (success && rc_val <= 2 /* higher is not relevant */) { _reference(); }
	return success;
}

bool BaseObject::unreference()
{
	uint32_t const rc_val{ m_refcount.unrefval() };
	bool const die{ rc_val == 0 };
	if (rc_val <= 1 /* higher is not relevant */) { _unreference(); }
	return die;
}

TYPE BaseObject::_get_typev() const noexcept { return get_type_static(); }

TYPE BaseObject::get_type_static() noexcept { return &g_type_static; }

TYPE BaseObject::get_type() const noexcept { return ((!!m_type) || (m_type = _get_typev())), m_type; }

void BaseObject::set_type(TYPE const & value) noexcept { m_type = value; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(BaseObject, t, TypeFlags_IsAbstract)
{
	t.tp_getattro = (getattrofunc)&BaseObject::generic_getattr;

	t.tp_setattro = (setattrofunc)&BaseObject::generic_setattr;

	t.tp_install = CLASS_INSTALLER(BaseObject, t)
	{
		return t

			.def("init_ref", &BaseObject::init_ref)
		
			.def("reference", &BaseObject::reference)

			.def("unreference", &BaseObject::unreference)

			.def("get_ref_count", &BaseObject::get_ref_count)

			.def("has_references", &BaseObject::has_references)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ BaseObject::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
{
	TYPE type{ ism::typeof(obj) };

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
		return ((DICT &)dict).lookup(name);
	}

	if (get) { return get(descr, obj, type); }

	if (descr) { return descr; }

	return nullptr;
}

OBJ BaseObject::generic_getattr(OBJ obj, OBJ name)
{
	return generic_getattr_with_dict(obj, name, nullptr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ BaseObject::generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict)
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

			return (((DICT &)dict)[name] = value), Error_None;
		}
		else
		{
			return Error_Unknown;
		}
	}
	else
	{
		return (((DICT &)dict)[name] = value), Error_None;
	}
}

Error_ BaseObject::generic_setattr(OBJ obj, OBJ name, OBJ value)
{
	return generic_setattr_with_dict(obj, name, value, nullptr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */