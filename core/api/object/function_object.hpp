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
		static void _bind_methods(TypeObject & t);

	public:
		vectorcallfunc m_vectorcall{};

		explicit FunctionObject(vectorcallfunc vectorcall) : base_type{ get_type_static() }, m_vectorcall{ vectorcall } {}

		NODISCARD vectorcallfunc get_dispatcher() const noexcept { return m_vectorcall; }

		void set_dispatcher(vectorcallfunc value) noexcept { m_vectorcall = value; }
	};
}

// function deleter
namespace ism { template <> struct DefaultDelete<api::FunctionObject> : DefaultDelete<api::BaseObject> {}; }

// function handle
namespace ism::api
{
	template <> class Handle<FunctionObject> : public BaseHandle<FunctionObject>
	{
		ISM_HANDLE_DEFAULT(FunctionObject);

	public:
		Handle() = default;

		~Handle() = default;

		NODISCARD OBJECT cpp_function() const noexcept; // cppfunction_object.hpp

		NODISCARD bool is_cpp_function() const noexcept { return (bool)cpp_function(); }
	};
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
