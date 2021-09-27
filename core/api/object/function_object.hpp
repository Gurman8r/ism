#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API FunctionObject : public Object
	{
		OBJECT_COMMON(FunctionObject, Object);

	public:
		OBJ m_dict{}, m_module{};

		vectorcallfunc m_vectorcall{};

		virtual ~FunctionObject() noexcept override = default;

		FunctionObject() noexcept : Object{} {}

		FunctionObject(vectorcallfunc vectorcall) noexcept : FunctionObject{} { m_vectorcall = vectorcall; }
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define OBJECT_CHECK_FUNCTION(o) (ism::api::isinstance<ism::FUNCTION>(o))

	// function ref
	MAKE_CUSTOM_REF(FunctionObject)
	{
		REF_COMMON(FunctionObject, OBJECT_CHECK_FUNCTION);

	public:
		NODISCARD OBJ cpp_function() const; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_