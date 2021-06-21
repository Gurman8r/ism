#include <core/api/object/cppfunction_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(CppFunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_base = typeof<FUNCTION>();

	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type)->OBJECT
	{
		return !obj ? self : METHOD({ self, obj, method_vectorcall });
	};
};

void CppFunctionObject::_bind_class(OBJECT scope)
{
	CLASS_<CPP_FUNCTION>(scope, "cpp_function", get_type_static())

		//.def(init<>())

		.def_property("__name__", [](CPP_FUNCTION self) { return self->m_rec.name; }, [](CPP_FUNCTION self, STR value) { self->m_rec.name = value; })

		.def_property("__doc__", [](CPP_FUNCTION self) { return self->m_rec.doc; }, [](CPP_FUNCTION self, STR value) { self->m_rec.doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */