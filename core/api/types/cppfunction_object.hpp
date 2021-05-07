#ifndef _ISM_API_TYPES_CPPFUNCTION_OBJECT_HPP_
#define _ISM_API_TYPES_CPPFUNCTION_OBJECT_HPP_

#include <core/api/detail/init.hpp>
#include <core/api/types/function_object.hpp>

// cpp_function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCppFunction : public CoreFunction
	{
		ISM_OBJECT_DEFAULT(CoreCppFunction, CoreFunction);

	protected:
		static void _bind_class(CoreType & t);

	private:
		detail::function_record m_data{};

	public:
		template <class Return, class ... Args, class ... Extra
		> CoreCppFunction(Return(*f)(Args...), Extra && ... extra) : self_type{ detail::function_record{
			f,
			f,
			FWD(extra)...
		} } {}

		template <class Func, class ... Extra, class = std::enable_if_t<mpl::is_lambda_v<Func>>
		> CoreCppFunction(Func && f, Extra && ... extra) : self_type{ detail::function_record{
			FWD(f),
			(mpl::function_signature_t<Func> *)0,
			FWD(extra)...
		} } {}
		
		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...), Extra && ... extra) : self_type{ detail::function_record{
			[f](Class * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) &, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) const, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class const *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) const &, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class const *, Args...))0,
			FWD(extra)...
		} } {}

		NODISCARD operator detail::function_record * () const { return const_cast<detail::function_record *>(&m_data); }

		NODISCARD auto name() const { return attr("__name__"); }

	protected:
		explicit CoreCppFunction(detail::function_record && value) noexcept
			: base_type{ type_static(), dispatcher }, m_data{ std::move(value) }
		{
		}

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			detail::function_record const & func{ ***CPP_FUNCTION(callable) };

			detail::function_call call{ func, (0 < argc ? argv[0] : nullptr) };

			return func.impl(call(argv, argc));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_CPPFUNCTION_OBJECT_HPP_
