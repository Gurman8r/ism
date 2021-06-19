#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function object
namespace ism::api
{
	class ISM_API api::FunctionObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(FunctionObject, BaseObject);

	protected:
		static void _bind_class(TypeObject & t);
	};
}

// function delete
namespace ism { template <> struct DefaultDelete<api::FunctionObject> : DefaultDelete<api::BaseObject> {}; }

// function check
#define ISM_FUNCTION_CHECK(o) (isinstance<FUNCTION>(o))

// function handle
namespace ism::api
{
	template <> class Handle<FunctionObject> : public BaseHandle<FunctionObject>
	{
		ISM_HANDLE_DEFAULT(FunctionObject, ISM_FUNCTION_CHECK);

	public:
		NODISCARD OBJECT cpp_function() const noexcept; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return (bool)cpp_function(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
