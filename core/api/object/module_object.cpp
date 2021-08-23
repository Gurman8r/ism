#include <core/api/object/module_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPLEMENTATION(ModuleObject, t, "module", TypeFlags_BaseType)
{
	t.tp_dictoffset = offsetof(ModuleObject, m_dict);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(ModuleObject((String)STR(args[0]))); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other)
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

	t.tp_getattro = (getattrofunc)module_getattro;

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<MODULE>(type)

			.def("__contains__", [](MODULE self, OBJ value) { return DICT(self->m_dict).contains(value); })

			.def_property("__dict__", [](MODULE self) { return self->m_dict; }, [](MODULE self, DICT value) { self->m_dict = value; })

			.def_property("__name__", [](MODULE self) { return self->m_name; }, [](MODULE self, STR value) { self->m_name = value; })

			;
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::module_getattro(MODULE m, OBJ name)
{
	return generic_getattr(m, name);
}

MODULE ism::create_extension_module(cstring name)
{
	DICT modules{ get_internals().modules };
	
	STR str_name{ name };
	
	return !modules.contains(str_name) ? modules[str_name] = MODULE({ name }) : MODULE{};
}

MODULE ism::import_module(cstring name)
{
	DICT modules{ get_internals().modules };

	return modules.lookup(STR{ name }, MODULE{});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */