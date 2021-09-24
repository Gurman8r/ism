#include <core/object/function_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMP(FunctionObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(FunctionObject); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<FunctionObject>(type)

			.def_readwrite("__dict__", &FunctionObject::m_dict)

			.def_readwrite("__module__", &FunctionObject::m_module)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */