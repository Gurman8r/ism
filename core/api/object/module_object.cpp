#include <core/api/object/module_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(ModuleObject, t)
{
	t.tp_name = "module";
	t.tp_size = sizeof(ModuleObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(ModuleObject, m_dict);
	
	t.tp_getattro = (getattrofunc)module_getattro;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (MODULE::check_(v))
		{
			return (*o == *v) ? 0 : util::compare(MODULE(o)->m_name, MODULE(v)->m_name);
		}
		else
		{
			return util::compare(*o, *v);
		}
	};

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((ModuleObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ModuleObject::_bind_class(TypeObject & t)
{
	CLASS_<MODULE>(&t, "module")

		//.def(init<>())

		.def("__contains__", [](MODULE self, OBJECT value) { return DICT(self->m_dict).contains(value); })

		.def_property("__name__", [](MODULE self) { return self->m_name; }, [](MODULE self, STR value) { self->m_name = value; })

		.def_property("__doc__", [](MODULE self) { return self->m_doc; }, [](MODULE self, STR value) { self->m_doc = value; })
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::module_getattro(MODULE m, OBJECT name)
{
	return generic_getattr(m, name);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */