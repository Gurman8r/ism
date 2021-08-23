#ifndef _ISM_METHOD_OBJECT_HPP_
#define _ISM_METHOD_OBJECT_HPP_

#include <core/api/object/function_object.hpp>

// method
namespace ism
{
	// method object
	class ISM_API MethodObject : public FunctionObject
	{
		ISM_OBJECT_DEFAULT(MethodObject, FunctionObject);

	public:
		OBJ m_func{}, m_self{};

		MethodObject() noexcept : MethodObject{ get_class() } {}

		MethodObject(OBJ func, OBJ self, vectorcallfunc vectorcall) : MethodObject{}
		{
			m_func = func;
			m_self = self;
			m_vectorcall = vectorcall;
		}
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<Object> {};

	// method check
#define ISM_CHECK_METHOD(o) (ism::isinstance<ism::METHOD>(o))

	// method handle
	template <> class Handle<MethodObject> : public Ref<MethodObject>
	{
		ISM_HANDLE(MethodObject, ISM_CHECK_METHOD);

	public:
	};
}

// functions
namespace ism
{
	ISM_API_FUNC(OBJ) method_vectorcall(OBJ callable, OBJ const * argv, size_t argc);
}

#endif // !_ISM_METHOD_OBJECT_HPP_
