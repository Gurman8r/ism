#include <core/api/object/module_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(ModuleObject, t, TypeFlags_BaseType)
{
	t.tp_dictoffset = offsetof(ModuleObject, m_dict);

	t.tp_getattro = (getattrofunc)api::module_getattro;

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return memnew(ModuleObject(STR(args[0])));
	};

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (MODULE::check_(other))
		{
			return (*self == *other) ? 0 : util::compare(MODULE(self)->m_name, MODULE(other)->m_name);
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_bind = BIND_CLASS_HELPER(ModuleObject, t)
	{
		return t

			.def("__contains__", [](ModuleObject const & self, OBJ const & value) { return self.m_dict.contains(value); })

			.def_readonly("__name__", &ModuleObject::m_name)

			.def_readonly("__dict__", &ModuleObject::m_dict)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::api::module_getattro(MODULE m, OBJ name)
{
	return generic_getattr(m, name);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */