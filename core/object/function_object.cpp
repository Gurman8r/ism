#include <core/object/function_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(FunctionObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_bind = MAKE_CLASS_BINDER(FunctionObject, t)
	{
		return t

			.def_readwrite("__dict__", &FunctionObject::m_dict)

			.def_readwrite("__module__", &FunctionObject::m_module)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */