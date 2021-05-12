#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/detail/initimpl.hpp>

// cpp_function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCppFunction : public CoreFunction
	{
		ISM_OBJECT_DEFAULT(CoreCppFunction, CoreFunction);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		detail::function_record m_cppfunction{};

	public:
		using storage_type = decltype(m_cppfunction);

		explicit CoreCppFunction(storage_type && value) noexcept : base_type{ &ob_type_static, dispatcher }, m_cppfunction{ std::move(value) } {}

		template <class Func, class ... Extra
		> CoreCppFunction(Func && f, Extra && ... extra) : self_type{ storage_type{ FWD(f), FWD(extra)... } } {}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_cppfunction); }

		NODISCARD OBJECT name() const { return attr("__name__"); }

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			detail::function_record const & func{ ***CPP_FUNCTION(callable) };
			
			detail::function_call call{ func, (0 < argc ? argv[0] : nullptr) };
			
			return func.impl(call(argv, argc));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
