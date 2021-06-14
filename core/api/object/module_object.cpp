#include <core/api/object/module_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(ModuleObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "module";
	t.tp_basicsize = sizeof(ModuleObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(ModuleObject, m_dict);

	t.tp_getattr = (getattrfunc)impl_getattr_string;
	t.tp_setattr = (setattrfunc)impl_setattr_string;
	t.tp_getattro = (getattrofunc)impl_getattr_object;
	t.tp_setattro = (setattrofunc)impl_setattr_object;

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

void ModuleObject::_bind_class(TypeObject & t)
{
	t.attr("__contains__") = CPP_FUNCTION([](MODULE self, OBJECT value) {
		return MODULE(self->m_dict)->contains(value);
	});

	//t.attr("__name__") = PROPERTY({
	//	CPP_FUNCTION([](MODULE self) { return self->m_name; }),
	//	CPP_FUNCTION([](MODULE self, STR value) { self->m_name = value; })
	//});
	//
	//t.attr("__doc__") = PROPERTY({
	//	CPP_FUNCTION([](MODULE self) { return self->m_doc; }),
	//	CPP_FUNCTION([](MODULE self, STR value) { self->m_doc = value; })
	//});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */