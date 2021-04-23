#include <core/api/types.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef cppfunction_getsets[] =
{
	{ "__dict__",
		(getter)[](OBJECT self, void *) -> OBJECT { return CPP_FUNCTION(self)->m_dict; },
	},
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).name = ***STR(value)), Error_None; }
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).doc = ***STR(value)), Error_None; }
	},
	{ /* sentinal */},
};

CoreType ism::_CoreCppFunction_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "cpp_function";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;

	t.tp_dict_offset = offsetof(CoreCppFunction, m_dict);
	t.tp_vectorcall_offset = offsetof(CoreCppFunction, m_vectorcall);

	t.tp_getattr = (getattrfunc)detail::getattr_string;
	t.tp_setattr = (setattrfunc)detail::setattr_string;
	t.tp_getattro = (getattrofunc)detail::getattr_object;
	t.tp_setattro = (setattrofunc)detail::setattr_object;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_alloc = (allocfunc)[](TYPE type, size_t nitems) -> OBJECT
	{
		size_t const size{ type->tp_basicsize + (nitems + 1) * type->tp_itemsize };

		return _post_initialize(::new (memalloc(size)) CoreObject{ type });
	};

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreCppFunction *)ptr); };

	t.tp_getsets = cppfunction_getsets;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef module_getsets[] =
{
	{ "__dict__",
		(getter)[](OBJECT self, void *) -> OBJECT { return MODULE(self)->m_dict; },
	},
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return MODULE(self)->m_name; },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (MODULE(self)->m_name = value), Error_None; }
	},
	{ /* sentinal */ },
};

static MethodDef module_methods[] =
{
	{ "__contains__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(MODULE(self)->m_dict->contains(value));
	} },
	{ /* sentinal */ },
};

CoreType ism::_CoreModule_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "module";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	
	t.tp_dict_offset = offsetof(CoreModule, m_dict);

	t.tp_getattr = (getattrfunc)detail::getattr_string;
	t.tp_setattr = (setattrfunc)detail::setattr_string;
	t.tp_getattro = (getattrofunc)detail::getattr_object;
	t.tp_setattro = (setattrofunc)detail::setattr_object;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		if (isinstance<MODULE>(value))
		{
			return (self.ptr() == value.ptr()) ? 0 : util::compare(MODULE(self)->m_name, MODULE(value)->m_name);
		}
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreModule *)ptr); };

	t.tp_getsets = module_getsets;
	t.tp_methods = module_methods;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreGeneric_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "generic_type";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_base = CoreType_Type;

	t.tp_dict_offset = offsetof(CoreGeneric, m_dict);
	t.tp_vectorcall_offset = offsetof(CoreGeneric, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreGeneric *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */