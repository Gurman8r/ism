#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/api/detail/call.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::detail
{
	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		using type = typename T;
		static void init(T &&, function_record &) {}
		static void init(T &&, CoreType &) {}
		static void precall(function_call &) {}
		static void postcall(function_call &, OBJECT) {}
	};
}

#define ATTRIBUTE_PROCESSOR(m_class) \
	struct ism::detail::process_attribute<m_class> : ism::detail::process_attribute_default<m_class>

#define ATTRIBUTE_PROCESSOR_BASE(m_class, m_inherits) \
	struct ism::detail::process_attribute<m_class> : ism::detail::process_attribute_default<m_inherits>

#define ATTRIBUTE_PROCESSOR_SFINAE(m_class, m_sfinae) \
	struct ism::detail::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::detail::process_attribute_default<m_class>

#define ATTRIBUTE_PROCESSOR_SFINAE_BASE(m_class, m_sfinae, m_inherits) \
	struct ism::detail::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::detail::process_attribute_default<m_inherits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// empty attribute
#define BASIC_ATTRIBUTE_E(m_class) \
	struct m_class final { \
		explicit m_class() noexcept {} \
	}

// object attribute
#define BASIC_ATTRIBUTE_O(m_class, m_value) \
	struct m_class final { \
		ism::CoreObject * const m_value; \
		explicit m_class(ism::CoreObject * value) : m_value{ value } {} \
		explicit m_class(ism::OBJECT value) : m_value{ *value } {} \
	}

// string attribute
#define BASIC_ATTRIBUTE_S(m_class, m_value) \
	struct m_class final { \
		char const * const m_value; \
		explicit m_class(char const * value) : m_value{ value } {} \
		explicit m_class(char * value) : m_value{ (char const *)value } {} \
	}

// bool attribute
#define BASIC_ATTRIBUTE_B(m_class, m_value) \
	struct m_class final { \
		bool const m_value; \
		explicit m_class(bool value) : m_value{ value } {} \
	}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	BASIC_ATTRIBUTE_S(name, value);
	template <> ATTRIBUTE_PROCESSOR(name) {
		static void init(function_record & r, name && a) { r.name = a.value; }
		static void init(CoreType & r, name && a) { r.tp_name = a.value; }
	};

	// doc
	BASIC_ATTRIBUTE_S(doc, value);
	template <> ATTRIBUTE_PROCESSOR(doc) {
		static void init(function_record & r, doc && a) { r.doc = a.value; }
		static void init(CoreType & r, doc && a) { r.tp_doc = a.value; }
	};
	template <> ATTRIBUTE_PROCESSOR(cstring) {
		static void init(function_record & r, cstring a) { r.doc = a; }
		static void init(CoreType & r, cstring a) { r.tp_doc = a; }
	};
	template <> ATTRIBUTE_PROCESSOR_BASE(char *, cstring) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ReturnPolicy
	template <> ATTRIBUTE_PROCESSOR(ReturnPolicy) {
		static void init(function_record & r, ReturnPolicy && a) { r.policy = a; }
	};

	// sibling
	BASIC_ATTRIBUTE_O(sibling, value);
	template <> ATTRIBUTE_PROCESSOR(sibling) {
		static void init(function_record & r, sibling && a) { r.sibling = a.value; }
	};

	// is_method
	BASIC_ATTRIBUTE_O(is_method, value);
	template <> ATTRIBUTE_PROCESSOR(is_method) {
		static void init(function_record & r, is_method && a) { r.is_method = true; r.scope = a.value; }
	};

	// scope
	BASIC_ATTRIBUTE_O(scope, value);
	template <> ATTRIBUTE_PROCESSOR(scope) {
		static void init(function_record & r, scope && a) { r.scope = a.value; }
	};

	// is_operator
	BASIC_ATTRIBUTE_E(is_operator);
	template <> ATTRIBUTE_PROCESSOR(is_operator) {
		static void init(function_record & r, is_operator && a) { r.is_operator = true; }
	};

	// is_constructor
	BASIC_ATTRIBUTE_E(is_constructor);
	template <> ATTRIBUTE_PROCESSOR(is_constructor) {
		static void init(function_record & r, is_constructor && a) { r.is_constructor = true; }
	};

	// prepend
	BASIC_ATTRIBUTE_E(prepend);
	template <> ATTRIBUTE_PROCESSOR(prepend) {
		static void init(function_record & r, prepend && a) { r.prepend = true; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// parent class
	template <class T> ATTRIBUTE_PROCESSOR_SFINAE(T, is_object_api_v<T>) {
		static void init(CoreType & r, T && a) { r.bases->append(typeof(FWD(a))); }
	};

	// multiple inheritance
	BASIC_ATTRIBUTE_E(multiple_inheritance);
	template <> ATTRIBUTE_PROCESSOR(multiple_inheritance) {
		static void init(CoreType & r, multiple_inheritance && a) { if (!r.tp_bases) { r.tp_bases = LIST(CoreList{}); } }
	};

	// dynamic_attr
	BASIC_ATTRIBUTE_E(dynamic_attr);
	template <> ATTRIBUTE_PROCESSOR(dynamic_attr) {
		static void init(CoreType & r, dynamic_attr && a) { if (!r.tp_dict) { r.tp_dict = DICT(CoreDict{}); } }
	};

	// is_final
	BASIC_ATTRIBUTE_E(is_final);
	template <> ATTRIBUTE_PROCESSOR(is_final) {
		static void init(CoreType & r, is_final && a) { /*r.tp_is_final = true;*/ }
	};

	// metaclass
	BASIC_ATTRIBUTE_O(metaclass, value);
	template <> ATTRIBUTE_PROCESSOR(metaclass) {
		static void init(CoreType & r, type && a) { r.ob_type = super_cast<CoreType>(a.value); }
	};

	// module_local
	BASIC_ATTRIBUTE_B(module_local, value);
	template <> ATTRIBUTE_PROCESSOR(module_local) {
		static void init(CoreType & r, module_local && a) { /*r.tp_module_local = a.value;*/ }
	};

	// arithmetic
	BASIC_ATTRIBUTE_E(arithmetic);
	template <> ATTRIBUTE_PROCESSOR(arithmetic) {
		static void init(CoreType & r, arithmetic && a) { /* nothing to do here */ }
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
			T guard{}; // Compose multiple guard types with left-to-right default-constructor order

			typename call_guard<Ts...>::type next{};
		};
	};

	template <class ... Ts> ATTRIBUTE_PROCESSOR(call_guard<Ts...>) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process_attributes
	template <class ... Args> struct process_attributes
	{
		template <class T> static void init(T & r, Args && ... args)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::init(r, FWD(args)), 0) ...);
		}

		static void precall(function_call & call)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::precall(call), 0) ...);
		}

		static void postcall(function_call & call, OBJECT retv)
		{
			SINK(0, (process_attribute<std::decay_t<Args>>::postcall(call, retv), 0) ...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> using is_call_guard = is_instantiation<call_guard, T>;

	template <class ... Extra
	> using extract_guard_t = typename exactly_one_t<is_call_guard, call_guard<>, Extra...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ATTR_HPP_
