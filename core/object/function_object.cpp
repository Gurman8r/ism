#include <core/object/function_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(FunctionObject, t, TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
	{
		t.tp_dictoffset = offsetof(FunctionObject, m_dict);

		t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

		t.tp_bind = CLASS_BINDER(FunctionObject, t)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}