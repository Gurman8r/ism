#ifndef _ISM_METHOD_OBJECT_HPP_
#define _ISM_METHOD_OBJECT_HPP_

#include <core/object/function_object.hpp>

// method
namespace ism
{
	// method object
	class ISM_API MethodObject : public FunctionObject
	{
		OBJECT_COMMON(MethodObject, FunctionObject);

	public:
		OBJ m_func{}, m_self{};

		MethodObject() noexcept : FunctionObject{} {}

		MethodObject(OBJ const & func, OBJ const & self, vectorcallfunc vectorcall) : FunctionObject{ vectorcall }
		{
			m_func = func;
			m_self = self;
		}
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<Object> {};

	// method check
#define OBJECT_CHECK_METHOD(o) (ism::isinstance<ism::METHOD>(o))

	// method handle
	DECL_CUSTOM_REF(MethodObject)
	{
		REF_COMMON(MethodObject, OBJECT_CHECK_METHOD);
	};
}

// functions
namespace ism
{
	ISM_API_FUNC(OBJ) method_vectorcall(OBJ callable, OBJ const * argv, size_t argc);
}

#endif // !_ISM_METHOD_OBJECT_HPP_
