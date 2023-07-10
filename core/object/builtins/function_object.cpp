#include <core/object/builtins/function_object.hpp>
#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(FunctionObject, t, TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
	{
		t.tp_dictoffset = offsetof(FunctionObject, m_dict);

		t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

		t.tp_bind = BIND_CLASS(FunctionObject, klass)
		{
			return klass
				.def_readwrite("__dict__", &FunctionObject::m_dict)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FunctionObject::FunctionObject(VectorCallFunc vectorcall)
		: m_dict{ DictRef::new_() }
		, m_vectorcall{ vectorcall }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}