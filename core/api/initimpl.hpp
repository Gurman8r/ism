#ifndef _ISM_INITIMPL_HPP_
#define _ISM_INITIMPL_HPP_

#include <core/api/attr.hpp>

namespace ism::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename Class> using Cpp = typename Class::value_type;

	template <typename Class> using Holder = typename Class::holder_type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// constructor
	template <class ... Args
	> struct Constructor
	{
		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [](Holder<Class> & holder, Args ... args)
			{
				::new(*holder) Cpp<Class>{ args... };
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra)
		{
			// TODO...
			return c;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// factory
	template <class Fn, class R = mpl::function_signature_t<Fn>, class ... Args
	> struct Factory
	{
		std::remove_reference_t<Fn> class_factory;

		Factory(Fn && fn) noexcept : class_factory{ FWD(fn) } {}

		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [fn = std::move(class_factory)](Holder<Class> & holder, Args ... args)
			{
				::new(*holder) Cpp<Class>{ fn(args...) };
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra)
		{
			// TODO...
			return c;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> NODISCARD auto init() -> initimpl::Constructor<Args...> { return {}; }

	template <class Func, class Ret = initimpl::Factory<Func>
	> NODISCARD auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INITIMPL_HPP_
