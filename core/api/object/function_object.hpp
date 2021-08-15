#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API FunctionObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(FunctionObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		OBJECT m_name{}, m_doc{}, m_qualname{};

		OBJECT m_dict{}, m_module{};

		int32_t m_flags{};

		vectorcallfunc m_vectorcall{};

		virtual ~FunctionObject() noexcept override = default;
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<BaseObject> {};

	// function check
#define ISM_FUNCTION_CHECK(o) (ism::isinstance<ism::FUNCTION>(o))

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
