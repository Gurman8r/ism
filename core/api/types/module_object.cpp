#include <core/api/types/module_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreModule::ob_class) { nullptr };

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

void CoreModule::_bind_class(CoreType & t)
{
	t.tp_name = "module";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_dict_offset = offsetof(CoreModule, m_dict);

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		if (isinstance<MODULE>(value))
		{
			return (self.ptr() == value.ptr()) ? 0 : util::compare(MODULE(self)->m_name, MODULE(value)->m_name);
		}
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreModule *)ptr); };

	t.tp_getsets = module_getsets;
	t.tp_methods = module_methods;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */