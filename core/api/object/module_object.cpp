#include <core/api/object/module_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(ModuleObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_dictoffset = offsetof(ModuleObject, m_dict);
	
	t.tp_getattro = (getattrofunc)module_getattro;

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
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
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ModuleObject::_bind_class(OBJ scope)
{
	CLASS_<MODULE>(scope, "module")

		.def(init<String const &>())

		.def("__contains__", [](MODULE self, OBJ value) { return DICT(self->m_dict).contains(value); })

		.def_property("__name__", [](MODULE self) { return self->m_name; }, [](MODULE self, STR value) { self->m_name = value; })

		.def_property("__doc__", [](MODULE self) { return self->m_doc; }, [](MODULE self, STR value) { self->m_doc = value; })
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::module_getattro(MODULE m, OBJ name)
{
	return generic_getattr(m, name);
}

MODULE ism::create_extension_module(cstring name)
{
	DICT modules{ get_script_server()->modules };
	
	STR str_name{ name };
	
	return !modules.contains(str_name) ? modules[str_name] = MODULE({ name }) : MODULE{};
}

MODULE ism::import_module(cstring name)
{
	return get_script_server()->modules.lookup(STR{ name }, MODULE{});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */