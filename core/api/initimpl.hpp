#ifndef _ISM_INITIMPL_HPP_
#define _ISM_INITIMPL_HPP_

#include <core/api/attr.hpp>

namespace ism::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename Class> using Cpp = typename Class::value_type;
	template <typename Class> using Alias = typename Class::type_alias;
	template <typename Class> using Holder = typename Class::holder_type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct constructor
	{
		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [](Holder<Class> h, Args ... args)
			{
				//::new (h.ptr()) Cpp<Class>{ args... };

				h.instance(args...);

				return h;
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [](Holder<Class> h, Args ... args)
			{
				// TODO...

				return h;
			}
			, attr::is_constructor(), FWD(extra)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class R = mpl::function_signature_t<Fn>, class ... Args
	> struct factory
	{
		std::remove_reference_t<Fn> class_factory;

		factory(Fn && fn) noexcept : class_factory{ FWD(fn) } {}

		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [fn = std::move(class_factory)](Holder<Class> h, Args ... args)
			{
				//::new (h.ptr()) Cpp<Class> { fn(args...) };

				h.instance(fn(args...));

				return h;
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__init__", [fn = std::move(class_factory)](Holder<Class> h, Args ... args)
			{
				// TODO...

				return h;
			}
			, attr::is_constructor(), FWD(extra)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> NODISCARD auto init() -> initimpl::constructor<Args...> { return {}; }

	template <class Func, class Ret = initimpl::factory<Func>
	> NODISCARD auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INITIMPL_HPP_
