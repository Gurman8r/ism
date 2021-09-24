#include <core/object/function_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(FunctionObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_bind = CLASS_BINDER(FunctionObject, c)
	{
		return c

			.def_readwrite("__dict__", &FunctionObject::m_dict)

			.def_readwrite("__module__", &FunctionObject::m_module)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */