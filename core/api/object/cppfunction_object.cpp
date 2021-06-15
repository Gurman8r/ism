#include <core/api/object/cppfunction_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(CppFunctionObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "cpp_function";
	t.tp_size = sizeof(CppFunctionObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;
	t.tp_base = typeof<FUNCTION>();

	t.tp_vectorcall_offset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_getattro = (getattrofunc)generic_getattr;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CppFunctionObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

void CppFunctionObject::_bind_class(TypeObject & t)
{
	t.tp_dict["__name__"] = PROPERTY({
		CPP_FUNCTION([](CPP_FUNCTION self) { return self->m_cppfunction.name; }),
		CPP_FUNCTION([](CPP_FUNCTION self, STR value) { self->m_cppfunction.name = value; })
	});
	
	t.tp_dict["__doc__"] = PROPERTY({
		CPP_FUNCTION([](CPP_FUNCTION self) { return self->m_cppfunction.doc; }),
		CPP_FUNCTION([](CPP_FUNCTION self, STR value) { self->m_cppfunction.doc = value; })
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */