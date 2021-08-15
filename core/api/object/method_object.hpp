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

	protected:
		static void _bind_class(OBJECT scope);

	public:
		OBJECT m_func{}, m_self{};

		MethodObject(OBJECT func, OBJECT self, vectorcallfunc vectorcall) : self_type{}
		{
			m_func = func;
			m_self = self;
			m_vectorcall = vectorcall;
		}
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<BaseObject> {};

	// method check
#define ISM_METHOD_CHECK(o) (ism::isinstance<ism::METHOD>(o))

	// method handle
	template <> class Handle<MethodObject> : public BaseHandle<MethodObject>
	{
		ISM_HANDLE_DEFAULT(MethodObject, ISM_METHOD_CHECK);

	public:
	};
}

// functions
namespace ism
{
	ISM_API_FUNC(OBJECT) method_vectorcall(OBJECT callable, OBJECT const * argv, size_t argc);
}

#endif // !_ISM_METHOD_OBJECT_HPP_
