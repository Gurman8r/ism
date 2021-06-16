#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_BUILTIN_TYPE(GenericObject, t)
{
	t.tp_name = "generic";
	t.tp_size = sizeof(GenericObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_getattro = (getattrofunc)generic_getattr;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((GenericObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

void GenericObject::_bind_methods(TypeObject & t)
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */