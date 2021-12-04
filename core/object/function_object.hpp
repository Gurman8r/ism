#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API FunctionObject : public Object
	{
		OBJECT_COMMON(FunctionObject, Object);

		friend class FUNCTION;

	public:
		OBJ m_dict{};

		vectorcallfunc m_vectorcall{};

		virtual ~FunctionObject() noexcept override = default;

		FunctionObject(vectorcallfunc vectorcall = nullptr);

		NODISCARD OBJ cpp_function() const; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define OBJECT_CHECK_FUNCTION(o) (ism::isinstance<ism::FUNCTION>(o))

	// function ref
	class FUNCTION : public Ref<FunctionObject>
	{
		REF_COMMON(FUNCTION, OBJECT_CHECK_FUNCTION);

	public:
		NODISCARD OBJ cpp_function() const { return m_ptr->cpp_function(); }

		NODISCARD bool is_cpp_function() const noexcept { return m_ptr->is_cpp_function(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_