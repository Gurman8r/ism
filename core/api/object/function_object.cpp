#include <core/api/object/function_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(FunctionObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "function";
	t.tp_basicsize = sizeof(FunctionObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;
	t.tp_base = typeof<OBJECT>();

	t.tp_vectorcall_offset = offsetof(FunctionObject, m_vectorcall);

	t.tp_getattr = (getattrfunc)impl_getattr_string;
	t.tp_setattr = (setattrfunc)impl_setattr_string;
	t.tp_getattro = (getattrofunc)impl_getattr_object;
	t.tp_setattro = (setattrofunc)impl_setattr_object;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((FunctionObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_class(TypeObject & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */