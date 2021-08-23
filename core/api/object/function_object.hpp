#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API FunctionObject : public Object
	{
		ISM_OBJECT(FunctionObject, Object);

	public:
		OBJ m_name{}, m_qualname{};

		OBJ m_dict{}, m_module{};

		vectorcallfunc m_vectorcall{};

		virtual ~FunctionObject() noexcept override = default;

		explicit FunctionObject(TYPE type) noexcept : Object{ type } {}

		FunctionObject() noexcept : FunctionObject{ get_class_static() } {}
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define ISM_CHECK_FUNCTION(o) (ism::isinstance<ism::FUNCTION>(o))

	// function handle
	template <> class Handle<FunctionObject> : public Ref<FunctionObject>
	{
		ISM_HANDLE(FunctionObject, ISM_CHECK_FUNCTION);

	public:
		NODISCARD OBJ cpp_function() const; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
