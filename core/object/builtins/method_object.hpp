#ifndef _ISM_METHOD_OBJECT_HPP_
#define _ISM_METHOD_OBJECT_HPP_

#include <core/object/builtins/function_object.hpp>

// method
namespace Ism
{
	// method object
	class ISM_API MethodObject : public FunctionObject
	{
		OBJECT_CLASS(MethodObject, FunctionObject);

		friend class MethodRef;

	public:
		ObjectRef m_func{}, m_self{};

		MethodObject() noexcept : FunctionObject{} {}

		MethodObject(ObjectRef const & func, ObjectRef const & self, VectorCallFunc vectorcall = &method_vectorcall)
			: FunctionObject{ vectorcall }
			, m_func		{ func }
			, m_self		{ self }
		{
		}

	public:
		static ObjectRef method_vectorcall(ObjectRef callable, ObjectRef const * argv, size_t argc);
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<Object> {};

	// method check
#define OBJECT_CHECK_METHOD(o) (isinstance<MethodRef>(o))

	// method ref
	class MethodRef : public Ref<MethodObject>
	{
		CUSTOM_REF(MethodRef, OBJECT_CHECK_METHOD);
	};
}

#endif // !_ISM_METHOD_OBJECT_HPP_
