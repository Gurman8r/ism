#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/api/call.hpp>

// i want to get rid of this system, but don't currently have any better ideas

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		using type = typename T;
		static void init(FunctionRecord &, T &&) {}
		static void precall(FunctionCall &) {}
		static void postcall(FunctionCall &, OBJ) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	struct name { cstring value; name(cstring value) : value{ value } {} };
	template <> struct process_attribute<name> : process_attribute_default<name>
	{
		static void init(FunctionRecord & r, name && a) noexcept { r.name = a.value; }
	};

	// return policy
	template <> struct process_attribute<ReturnValuePolicy_> : process_attribute_default<ReturnValuePolicy_>
	{
		static void init(FunctionRecord & r, ReturnValuePolicy_ && a) noexcept { r.policy = a; }
	};

	// sibling
	struct sibling { Object * value; sibling(Object * value) : value{ value } {} sibling(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<sibling> : process_attribute_default<sibling>
	{
		static void init(FunctionRecord & r, sibling && a) noexcept { r.sibling = a.value; }
	};

	// is_method
	struct is_method { Object * value; is_method(Object * value) : value{ value } {} is_method(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<is_method> : process_attribute_default<is_method>
	{
		static void init(FunctionRecord & r, is_method && a) noexcept { r.is_method = true; r.scope = a.value; }
	};

	// scope
	struct scope { Object * value; scope(Object * value) : value{ value } {} scope(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<scope> : process_attribute_default<scope>
	{
		static void init(FunctionRecord & r, scope && a) noexcept { r.scope = a.value; }
	};

	// is_operator
	struct is_operator {};
	template <> struct process_attribute<is_operator> : process_attribute_default<is_operator>
	{
		static void init(FunctionRecord & r, is_operator && a) noexcept { r.is_operator = true; }
	};

	// is_constructor
	struct is_constructor {};
	template <> struct process_attribute<is_constructor> : process_attribute_default<is_constructor>
	{
		static void init(FunctionRecord & r, is_constructor && a) noexcept { r.is_constructor = true; }
	};

	// prepend
	struct prepend {};
	template <> struct process_attribute<prepend> : process_attribute_default<prepend> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// call guard
	template <class... Ts> struct call_guard;

	template <> struct call_guard<> { using type = void_type; };

	template <class T> struct call_guard<T>
	{
		static_assert(std::is_default_constructible_v<T>, "The guard type must be default constructible");

		using type = T;
	};

	template <class T, class ... Ts> struct call_guard<T, Ts...>
	{
		struct type
		{
			T guard{}; // compose multiple guard types with left-to-right default-newfunc order

			typename call_guard<Ts...>::type next{};
		};
	};

	template <class ... Ts> struct process_attribute<call_guard<Ts...>> : process_attribute_default<call_guard<Ts...>> {};

	template <class T
	> ALIAS(is_call_guard) mpl::is_instantiation<call_guard, T>;

	template <class ... Extra
	> ALIAS(extract_guard_t) mpl::exactly_one_t<is_call_guard, call_guard<>, Extra...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process attributes
	template <class ... Args> struct process_attributes
	{
		template <class T> static void init(T & r, Args && ... args)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::init(r, FWD(args)), 0) ...);
		}

		static void precall(FunctionCall & call)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::precall(call), 0) ...);
		}

		static void postcall(FunctionCall & call, OBJ retv)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::postcall(call, retv), 0) ...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ATTR_HPP_
