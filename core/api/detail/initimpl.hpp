#ifndef _ISM_INITIMPL_DETAIL_HPP_
#define _ISM_INITIMPL_DETAIL_HPP_

#include <core/api/detail/class.hpp>

namespace ism::api::initimpl
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
				v_h->holder().reset(memnew(Cpp<Class>{ FWD(args)... }));
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
				v_h->holder().reset(func(FWD(args)...));
			}
			, is_constructor{}, FWD(extra)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> NODISCARD auto init() -> api::initimpl::constructor<Args...> { return {}; }

	template <class Func, class Ret = api::initimpl::factory<Func>
	> NODISCARD auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INITIMPL_DETAIL_HPP_
