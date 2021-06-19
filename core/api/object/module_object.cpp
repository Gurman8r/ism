#include <core/api/object/module_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(ModuleObject, t)
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
		if (isinstance<MODULE>(v))
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

void ModuleObject::_bind_methods(TypeObject & t)
{
	t.tp_dict["__contains__"] = CPP_FUNCTION([](MODULE self, OBJECT value)
	{
		return DICT(self->m_dict).contains(value);
	});

	t.tp_dict["__name__"] = PROPERTY({
		[](MODULE self) { return self->m_name; },
		[](MODULE self, STR value) { self->m_name = value; } });
	
	t.tp_dict["__doc__"] = PROPERTY({
		[](MODULE self) { return self->m_doc; },
		[](MODULE self, STR value) { self->m_doc = value; } });
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::api::module_getattro(MODULE m, OBJECT name)
{
	return generic_getattr(m, name);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */