#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	// function object
	class ISM_API FunctionObject : public Object
	{
		ISM_OBJECT_DEFAULT(FunctionObject, Object);

	protected:
		static void _bind_class(OBJ scope);

	public:
		OBJ m_name{}, m_doc{}, m_qualname{};

		OBJ m_dict{}, m_module{};

		int32_t m_flags{};

		vectorcallfunc m_vectorcall{};

		FunctionObject() : base_type{ get_class() } {}

		virtual ~FunctionObject() noexcept override = default;
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define ISM_CHECK_FUNCTION(o) (ism::isinstance<ism::FUNCTION>(o))

	// function handle
	template <> class Handle<FunctionObject> : public Ref<FunctionObject>
	{
		ISM_HANDLE_DEFAULT(FunctionObject, ISM_CHECK_FUNCTION);

	public:
		NODISCARD OBJ cpp_function() const; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
