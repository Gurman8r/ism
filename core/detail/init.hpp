#ifndef _ISM_INIT_HPP_
#define _ISM_INIT_HPP_

#include <core/detail/attr.hpp>

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
			return c.def("__new__", [](Args ... args)
			{
				return Holder<Class>{ Cpp<Class>{ args... } };
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__new__", [](Args ... args)
			{
				return OBJ(/* TODO */);
			}
			, attr::is_constructor(), FWD(extra)...);
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
			return c.def("__new__", [func = std::move(class_factory)](Args ... args)
			{
				return Holder<Class>{ func(args...) };
			}
			, attr::is_constructor(), FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra)
		{
			return c.def("__new__", [func = std::move(class_factory)](Args ... args)
			{
				return OBJ(/* TODO */);
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
	> NODISCARD auto init() -> initimpl::Constructor<Args...> { return {}; }

	template <class Func, class Ret = initimpl::Factory<Func>
	> NODISCARD auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INIT_HPP_
