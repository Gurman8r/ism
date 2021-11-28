#include <core/api/object/function_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(FunctionObject, t, TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	CLASS_DEFINITION(FunctionObject, t)
	{
		return t

			.def_readwrite("__dict__", &FunctionObject::m_dict)
			
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

FunctionObject::FunctionObject(vectorcallfunc vectorcall) : Object{}, m_dict{ DICT::new_() }, m_vectorcall{ vectorcall }
{
}

OBJ FunctionObject::cpp_function() const
{
	return CPP_FUNCTION::check_((Object *)this) ? CPP_FUNCTION((CppFunctionObject *)this) : nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */