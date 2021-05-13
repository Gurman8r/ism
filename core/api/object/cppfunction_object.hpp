#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/detail/initimpl.hpp>

// cpp_function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCppFunction : public CoreFunction
	{
		ISM_OBJECT(CoreCppFunction, CoreFunction);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		detail::function_record m_cppfunction{};

		using storage_type = decltype(m_cppfunction);

		explicit CoreCppFunction(storage_type && value) noexcept : base_type{ &ob_type_static, dispatcher }, m_cppfunction{ std::move(value) } {}

		template <class Func, class ... Extra
		> CoreCppFunction(Func && f, Extra && ... extra) : self_type{ storage_type{ FWD(f), FWD(extra)... } } {}

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_cppfunction); }

		NODISCARD OBJECT name() const { return attr("__name__"); }

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			if (!callable) { return nullptr; }

			detail::function_record const & func{ **super_cast<CoreCppFunction>(*callable) };
			
			detail::function_call call{ func, (0 < argc ? argv[0] : nullptr) };
			
			return func.impl(call(argv, argc));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreCppFunction> : public BaseHandle<CoreCppFunction>
	{
		ISM_HANDLE(CoreCppFunction);
	
	public:
		Handle() = default;
	
		~Handle() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
