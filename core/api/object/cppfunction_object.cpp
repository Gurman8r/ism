#include <core/api/object/cppfunction_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(CppFunctionObject, t)
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

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type)->OBJECT
	{
		return !obj ? self : METHOD({ self, obj, method_vectorcall });
	};
};

void CppFunctionObject::_bind_class(TypeObject & t)
{
	CLASS_<CPP_FUNCTION>(&t, "cpp_function")

		//.def(init<>())

		.def_property("__name__", [](CPP_FUNCTION self) { return self->m_func.name; }, [](CPP_FUNCTION self, STR value) { self->m_func.name = value; })

		.def_property("__doc__", [](CPP_FUNCTION self) { return self->m_func.doc; }, [](CPP_FUNCTION self, STR value) { self->m_func.doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */