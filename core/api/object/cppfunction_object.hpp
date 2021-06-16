#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/detail/initimpl.hpp>

// cppfunction object
namespace ism::api
{
	class ISM_API CppFunctionObject : public FunctionObject
	{
		ISM_OBJECT_CVT(CppFunctionObject, FunctionObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		function_record m_func{};

		using storage_type = decltype(m_func);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_func); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_func); }

		CppFunctionObject() noexcept : base_type{ get_type_static() } { m_vectorcall = dispatcher; }

		explicit CppFunctionObject(storage_type const & value) : self_type{} { m_func = value; }

		explicit CppFunctionObject(storage_type && value) noexcept : self_type{} { m_func = std::move(value); }

		template <class Func, class ... Extra
		> CppFunctionObject(Func && f, Extra && ... extra) : self_type{ storage_type{ FWD(f), FWD(extra)... } } {}

	protected:
		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			if (!callable) { return nullptr; }

			function_record const & func{ **super_cast<CppFunctionObject>(*callable) };

			function_call call{ func, (0 < argc ? argv[0] : nullptr) };

			return func.impl(call(argv, argc));
		}
	};
}

// cppfunction deleter
namespace ism { template <> struct DefaultDelete<api::CppFunctionObject> : DefaultDelete<api::BaseObject> {}; }

// cppfunction handle
namespace ism::api
{
	template <> class Handle<CppFunctionObject> : public BaseHandle<CppFunctionObject>
	{
		ISM_HANDLE_DEFAULT(CppFunctionObject);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CppFunctionObject::storage_type;

		NODISCARD OBJECT name() const { return attr("__name__"); }
	};
}

namespace ism::api
{
	NODISCARD inline OBJECT Handle<FunctionObject>::cpp_function() const noexcept
	{
		return isinstance<CPP_FUNCTION>(*this) ? CPP_FUNCTION(*this) : nullptr;
	}
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
