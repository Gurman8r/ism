#ifndef _ISM_INITIMPL_HPP_
#define _ISM_INITIMPL_HPP_

#include <core/api/detail/class.hpp>

namespace ism::detail::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename Class> using Cpp = typename Class::value_type;
	template <typename Class> using Alias = typename Class::type_alias;
	template <typename Class> using Holder = typename Class::holder_type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct constructor
	{
		template <class Class, class ... Extra
		> static Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [](Handle<Class> v_h, Args ... args)
			{
				v_h->holder().reset(memnew(Cpp<Class>{ std::forward<Args>(args)... }));
			}
			, is_constructor{}, FWD(extra)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Func, class = mpl::function_signature_t<Func>
	> struct factory;

	template <class Func, class R, class ... Args
	> struct factory<Func, R(Args...)>
	{
		std::remove_reference_t<Func> class_factory;

		factory(Func && func) noexcept : class_factory{ FWD(func) } {}

		template <class Class, class ... Extra
		> Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [func = std::move(class_factory)](Handle<Class> v_h, Args ... args)
			{
				v_h->holder().reset(func(std::forward<Args>(args)...));
			}
			, is_constructor{}, FWD(extra)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> NODISCARD auto init() -> detail::initimpl::constructor<Args...> { return {}; }

	template <class Func, class Ret = detail::initimpl::factory<Func>
	> NODISCARD auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INITIMPL_HPP_
