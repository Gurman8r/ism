#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/detail/call.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::attr
{
	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		using type = typename T;
		static void init(T &&, FunctionRecord &) {}
		static void precall(FunctionCall &) {}
		static void postcall(FunctionCall &, OBJ) {}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// process attribute default
#define PROCESS_ATTRIBUTE(m_class) \
	struct ism::attr::process_attribute<m_class> : ism::attr::process_attribute_default<m_class>

// process attribute base
#define PROCESS_ATTRIBUTE_BASE(m_class, m_inherits) \
	struct ism::attr::process_attribute<m_class> : ism::attr::process_attribute_default<m_inherits>

// process attribute sfinae
#define PROCESS_ATTRIBUTE_SFINAE(m_class, m_sfinae) \
	struct ism::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::attr::process_attribute_default<m_class>

// process attribute base/sfinae
#define PROCESS_ATTRIBUTE_BASE_SFINAE(m_class, m_sfinae, m_inherits) \
	struct ism::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::attr::process_attribute_default<m_inherits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct name { cstring value; name(cstring value) : value{ value } {} };
	
	struct sibling { Object * value; sibling(Object * value) : value{ value } {} sibling(OBJ value) : value{ *value } {} };
	
	struct is_method { Object * value; is_method(Object * value) : value{ value } {} is_method(OBJ value) : value{ *value } {} };
	
	struct scope { Object * value; scope(Object * value) : value{ value } {} scope(OBJ value) : value{ *value } {} };
	
	struct is_operator {};
	
	struct is_constructor {};
	
	struct prepend {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	template <> PROCESS_ATTRIBUTE(name)
	{
		static void init(FunctionRecord & r, name && a) noexcept { r.name = a.value; }
	};

	// ReturnPolicy
	template <> PROCESS_ATTRIBUTE(ReturnPolicy)
	{
		static void init(FunctionRecord & r, ReturnPolicy && a) noexcept { r.policy = a; }
	};

	// sibling
	template <> PROCESS_ATTRIBUTE(sibling)
	{
		static void init(FunctionRecord & r, sibling && a) noexcept { r.sibling = a.value; }
	};

	// is_method
	template <> PROCESS_ATTRIBUTE(is_method)
	{
		static void init(FunctionRecord & r, is_method && a) noexcept { r.is_method = true; r.scope = a.value; }
	};

	// scope
	template <> PROCESS_ATTRIBUTE(scope)
	{
		static void init(FunctionRecord & r, scope && a) noexcept { r.scope = a.value; }
	};

	// is_operator
	template <> PROCESS_ATTRIBUTE(is_operator)
	{
		static void init(FunctionRecord & r, is_operator && a) noexcept { r.is_operator = true; }
	};

	// is_constructor
	template <> PROCESS_ATTRIBUTE(is_constructor)
	{
		static void init(FunctionRecord & r, is_constructor && a) noexcept { r.is_constructor = true; }
	};

	// prepend
	template <> PROCESS_ATTRIBUTE(prepend)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// call_guard
	template <class... Ts> struct call_guard;

	template <> struct call_guard<> { using type = void_type; };

	template <class T> struct call_guard<T>
	{
		static_assert(std::is_default_constructorible<T>::value, "The guard type must be default constructible");

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

	template <class ... Ts> PROCESS_ATTRIBUTE(call_guard<Ts...>) {};

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

	template <class T
	> using is_call_guard = mpl::is_instantiation<call_guard, T>;

	template <class ... Extra
	> using extract_guard_t = typename mpl::exactly_one_t<is_call_guard, call_guard<>, Extra...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ATTR_HPP_
