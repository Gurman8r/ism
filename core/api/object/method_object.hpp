#ifndef _ISM_METHOD_OBJECT_HPP_
#define _ISM_METHOD_OBJECT_HPP_

#include <core/api/object/function_object.hpp>

// method object
namespace ism::api
{
	class ISM_API MethodObject : public FunctionObject
	{
		ISM_OBJECT_DEFAULT(MethodObject, FunctionObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		OBJECT m_func{}, m_self{};

		vectorcallfunc m_vectorcall{};

		MethodObject(OBJECT func, OBJECT self, vectorcallfunc vectorcall) : base_type{ get_type_static() }
		{
			m_func = func;
			m_self = self;
			m_vectorcall = vectorcall;
		}
	};
}

// method delete
namespace ism { template <> struct DefaultDelete<api::MethodObject> : DefaultDelete<api::BaseObject> {}; }

// method handle
namespace ism::api
{
	template <> class Handle<MethodObject> : public BaseHandle<MethodObject>
	{
		ISM_HANDLE(MethodObject);

	public:
		Handle() = default;

		~Handle() = default;
	};
}

// functions
namespace ism::api
{
	ISM_API_FUNC(OBJECT) method_vectorcall(OBJECT callable, OBJECT const * argv, size_t argc);
}

#endif // !_ISM_METHOD_OBJECT_HPP_
