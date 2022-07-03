#include <core/object/function_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(FunctionObject, t, TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_install = CLASS_INSTALLER(FunctionObject, t)
	{
		return t

			.def_readwrite("__dict__", &FunctionObject::m_dict)
			
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

FunctionObject::FunctionObject(vectorcallfunc vectorcall) : m_dict{ DICT::new_() }, m_vectorcall{ vectorcall }
{
}

OBJ FunctionObject::cpp_function() const
{
	return CPP_FUNCTION::check_((BaseObject *)this) ? CPP_FUNCTION((CppFunctionObject *)this) : nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */