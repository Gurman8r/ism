#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/object/call.hpp>

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process attribute
	template <class T, class SFINAE = void> struct process_attribute;

	// process attribute default
	template <class T> struct process_attribute_default {
		using type = typename T;
		static void init(FunctionRecord &, T &&) noexcept {}
		static void precall(FunctionCall &) noexcept {}
		static void postcall(FunctionCall &, OBJ) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	struct name { cstring value; name(cstring value) : value{ value } {} };
	template <> struct process_attribute<name> : process_attribute_default<name> {
		static void init(FunctionRecord & r, name const & a) noexcept { r.name = a.value; }
	};

	// return policy
	template <> struct process_attribute<ReturnValuePolicy_> : process_attribute_default<ReturnValuePolicy_> {
		static void init(FunctionRecord & r, ReturnValuePolicy_ const & a) noexcept { r.policy = a; }
	};

	// sibling
	struct sibling { Object * value; sibling(Object * value) : value{ value } {} sibling(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<sibling> : process_attribute_default<sibling> {
		static void init(FunctionRecord & r, sibling const & a) noexcept { r.sibling = a.value; }
	};

	// is_method
	struct is_method { Object * value; is_method(Object * value) : value{ value } {} is_method(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<is_method> : process_attribute_default<is_method> {
		static void init(FunctionRecord & r, is_method const & a) noexcept { r.is_method = true; r.scope = a.value; }
	};

	// scope
	struct scope { Object * value; scope(Object * value) : value{ value } {} scope(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<scope> : process_attribute_default<scope> {
		static void init(FunctionRecord & r, scope const & a) noexcept { r.scope = a.value; }
	};

	// is_operator
	struct is_operator {};
	template <> struct process_attribute<is_operator> : process_attribute_default<is_operator> {
		static void init(FunctionRecord & r, is_operator const & a) noexcept { r.is_operator = true; }
	};

	// is_constructor
	struct is_constructor {};
	template <> struct process_attribute<is_constructor> : process_attribute_default<is_constructor> {
		static void init(FunctionRecord & r, is_constructor const & a) noexcept { r.is_constructor = true; }
	};

	// prepend
	struct prepend {};
	template <> struct process_attribute<prepend> : process_attribute_default<prepend> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// call guard
	/*
	A call policy which places one or more guard variables (``Ts...``) around the function call.
	
	For example, this definition:
		m.def("foo", foo, attr::call_guard<T>());
	
	is equivalent to the following pseudocode:
		m.def("foo", [](args...) {
			T scope_guard;
			return foo(args...); // forwarded arguments
		});
	*/
	template <class... Ts> struct call_guard;

	template <> struct call_guard<> { using type = mpl::void_type; };

	template <class T
	> struct call_guard<T>
	{
		static_assert(std::is_default_constructible_v<T>, "The guard type must be default constructible");

		using type = T;
	};

	template <class T, class ... Ts
	> struct call_guard<T, Ts...>
	{
		struct type
		{
			T guard{}; // compose multiple guard types with left-to-right default-NewFunc order

			typename call_guard<Ts...>::type next{};
		};
	};

	template <class ... Ts
	> struct process_attribute<call_guard<Ts...>> : process_attribute_default<call_guard<Ts...>> {};

	// is call guard
	template <class T
	> using is_call_guard_t = mpl::is_instantiation_t<call_guard, T>;
	
	// extract call guard
	template <class ... Extra
	> using extract_guard_t = typename mpl::exactly_one_t<is_call_guard_t, call_guard<>, Extra...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process attributes
	template <class ... Args> struct process_attributes
	{
		template <class T> static void init(T & r, Args && ... args) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::init(r, FWD(args)), 0) ...);
		}

		static void precall(FunctionCall & call) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::precall(call), 0) ...);
		}

		static void postcall(FunctionCall & call, OBJ retv) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::postcall(call, retv), 0) ...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ATTR_HPP_
