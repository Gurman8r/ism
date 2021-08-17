#include <core/api/object/module_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(ModuleObject, t)
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

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		VERIFY(LIST::check_(args));
		VERIFY(STR::check_(args[0]));
		return holder_type(self_type{ STR(args[0]).c_str() });
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ModuleObject::_bind_class(OBJ scope)
{
	CLASS_<MODULE>(scope, "module", get_class())

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */