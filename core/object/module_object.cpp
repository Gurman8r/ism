#include <core/object/module_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ModuleObject, t)
	{
		t.tp_dictoffset = offsetof(ModuleObject, m_dict);

		t.tp_getattro = (getattrofunc)&ModuleObject::module_getattro;

		t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
		{
			return memnew(ModuleObject(STR(args[0])));
		};

		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> i32
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

		t.tp_bind = CLASS_INSTALLER(ModuleObject, t)
		{
			return t
				.def("__contains__", [](ModuleObject const & self, OBJ const & value) { return self.m_dict.contains(value); })
				.def_readonly("__name__", &ModuleObject::m_name)
				.def_readonly("__dict__", &ModuleObject::m_dict)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MODULE ModuleObject::def_submodule(cstring name)
	{
		return nullptr;
	}

	void ModuleObject::reload()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJ ModuleObject::module_getattro(MODULE m, OBJ name)
	{
		return generic_getattr(m, name);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}