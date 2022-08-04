#ifndef _ISM_METHOD_OBJECT_HPP_
#define _ISM_METHOD_OBJECT_HPP_

#include <core/object/function_object.hpp>

// method
namespace ism
{
	// method object
	class ISM_API MethodObject : public FunctionObject
	{
		OBJECT_CLASS(MethodObject, FunctionObject);

		friend class METHOD;

	public:
		OBJ m_func{}, m_self{};

		MethodObject() noexcept : FunctionObject{} {}

		MethodObject(OBJ const & func, OBJ const & self, vectorcallfunc vectorcall = &method_vectorcall)
			: FunctionObject{ vectorcall }
			, m_func		{ func }
			, m_self		{ self }
		{
		}

	public:
		static OBJ method_vectorcall(OBJ callable, OBJ const * argv, size_t argc);
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<Object> {};

	// method check
#define OBJECT_CHECK_METHOD(o) (isinstance<METHOD>(o))

	// method ref
	class METHOD : public Ref<MethodObject>
	{
		REF_CLASS(METHOD, OBJECT_CHECK_METHOD);
	};
}

#endif // !_ISM_METHOD_OBJECT_HPP_
