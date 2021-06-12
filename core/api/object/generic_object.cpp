#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(GenericObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "generic_type";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(GenericObject, m_dict);
	t.tp_vectorcall_offset = offsetof(GenericObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((GenericObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void GenericObject::_bind_methods(TypeObject & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */