#ifndef _ISM_INITIMPL_HPP_
#define _ISM_INITIMPL_HPP_

#include <core/object/attr.hpp>

namespace ism::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// c++ type (Object)
	template <class Class> using Cpp = typename Class::value_type;

	// container type (Ref)
	template <class Class> using Holder = typename Class::holder_type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// constructor
	template <class ... Args
	> struct Constructor
	{
		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra const & ... extra)
		{
			return c.def("__new__", [](Args ... args)
			{
				return Holder<Class>::new_(args...);
			}
			, extra...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra const & ... extra)
		{
			static_assert(!"NOT IMPLEMENTED");
			return c; // TODO
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
		> Class & execute(Class & c, Extra const & ... extra)
		{
			return c.def("__new__", [func = std::move(class_factory)](Args ... args)
			{
				return Holder<Class>::new_(func(args...));
			}
			, extra...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra const & ... extra)
		{
			static_assert(!"NOT IMPLEMENTED");
			return c; // TODO
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> auto init() -> initimpl::Constructor<Args...> { return {}; }

	template <class Func, class Ret = initimpl::Factory<Func>
	> auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INITIMPL_HPP_
