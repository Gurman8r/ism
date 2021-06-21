#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API ism::FunctionObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(FunctionObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		OBJECT m_dict;

		vectorcallfunc m_vectorcall;
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<BaseObject> {};

	// function check
#define ISM_FUNCTION_CHECK(o) (isinstance<FUNCTION>(o))

	// function handle
	template <> class Handle<FunctionObject> : public BaseHandle<FunctionObject>
	{
		ISM_HANDLE_DEFAULT(FunctionObject, ISM_FUNCTION_CHECK);

	public:
		NODISCARD OBJECT cpp_function() const; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
