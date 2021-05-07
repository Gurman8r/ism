#ifndef _ISM_API_DETAIL_ATTR_HPP_
#define _ISM_API_DETAIL_ATTR_HPP_

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DECL_BASIC_PROCESSOR(m_class) \
	struct ism::detail::process_attribute<m_class> : ism::detail::process_attribute_default<m_class>

#define DECL_BASIC_PROCESSOR_BASE(m_class, m_inherits) \
	struct ism::detail::process_attribute<m_class> : ism::detail::process_attribute_default<m_inherits>

#define DECL_BASIC_PROCESSOR_SFINAE(m_class, m_sfinae) \
	struct ism::detail::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::detail::process_attribute_default<m_class>

#define DECL_BASIC_PROCESSOR_SFINAE_BASE(m_class, m_sfinae, m_inherits) \
	struct ism::detail::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::detail::process_attribute_default<m_inherits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// empty attribute
#define DECL_BASIC_ATTRIBUTE_E(m_class) \
	struct m_class final { \
		explicit m_class() noexcept {} \
	}

// object attribute
#define DECL_BASIC_ATTRIBUTE_O(m_class, m_value) \
	struct m_class final { \
		ism::CoreObject * const m_value; \
		explicit m_class(ism::CoreObject * value) : m_value{ value } {} \
		explicit m_class(ism::OBJECT value) : m_value{ *value } {} \
	}

// string attribute
#define DECL_BASIC_ATTRIBUTE_S(m_class, m_value) \
	struct m_class final { \
		char const * const m_value; \
		explicit m_class(char const * value) : m_value{ value } {} \
		explicit m_class(char * value) : m_value{ (char const *)value } {} \
	}

// bool attribute
#define DECL_BASIC_ATTRIBUTE_B(m_class, m_value) \
	struct m_class final { \
		bool const m_value; \
		explicit m_class(bool value) : m_value{ value } {} \
	}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	DECL_BASIC_ATTRIBUTE_S(name, value);
	template <> DECL_BASIC_PROCESSOR(name) {
		static void init(function_record & r, name && a) { r.name = a.value; }
		static void init(CoreType & r, name && a) { r.tp_name = a.value; }
	};

	// doc
	DECL_BASIC_ATTRIBUTE_S(doc, value);
	template <> DECL_BASIC_PROCESSOR(doc) {
		static void init(function_record & r, doc && a) { r.doc = a.value; }
		static void init(CoreType & r, doc && a) { r.tp_doc = a.value; }
	};
	template <> DECL_BASIC_PROCESSOR(cstring) {
		static void init(function_record & r, cstring a) { r.doc = a; }
		static void init(CoreType & r, cstring a) { r.tp_doc = a; }
	};
	template <> DECL_BASIC_PROCESSOR_BASE(char *, cstring) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ReturnPolicy
	template <> DECL_BASIC_PROCESSOR(ReturnPolicy) {
		static void init(function_record & r, ReturnPolicy && a) { r.policy = a; }
	};

	// sibling
	DECL_BASIC_ATTRIBUTE_O(sibling, value);
	template <> DECL_BASIC_PROCESSOR(sibling) {
		static void init(function_record & r, sibling && a) { r.sibling = a.value; }
	};

	// is_method
	DECL_BASIC_ATTRIBUTE_O(is_method, value);
	template <> DECL_BASIC_PROCESSOR(is_method) {
		static void init(function_record & r, is_method && a) { r.is_method = true; r.scope = a.value; }
	};

	// scope
	DECL_BASIC_ATTRIBUTE_O(scope, value);
	template <> DECL_BASIC_PROCESSOR(scope) {
		static void init(function_record & r, scope && a) { r.scope = a.value; }
	};

	// is_operator
	DECL_BASIC_ATTRIBUTE_E(is_operator);
	template <> DECL_BASIC_PROCESSOR(is_operator) {
		static void init(function_record & r, is_operator && a) { r.is_operator = true; }
	};

	// is_constructor
	DECL_BASIC_ATTRIBUTE_E(is_constructor);
	template <> DECL_BASIC_PROCESSOR(is_constructor) {
		static void init(function_record & r, is_constructor && a) { r.is_constructor = true; }
	};

	// prepend
	DECL_BASIC_ATTRIBUTE_E(prepend);
	template <> DECL_BASIC_PROCESSOR(prepend) {
		static void init(function_record & r, prepend && a) { r.prepend = true; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// parent class
	template <class T> DECL_BASIC_PROCESSOR_SFINAE(T, is_object_api_v<T>) {
		static void init(CoreType & r, T && a) { r.bases->append(typeof(FWD(a))); }
	};

	// multiple inheritance
	DECL_BASIC_ATTRIBUTE_E(multiple_inheritance);
	template <> DECL_BASIC_PROCESSOR(multiple_inheritance) {
		static void init(CoreType & r, multiple_inheritance && a) { if (!r.tp_bases) { r.tp_bases = LIST(CoreList{}); } }
	};

	// dynamic_attr
	DECL_BASIC_ATTRIBUTE_E(dynamic_attr);
	template <> DECL_BASIC_PROCESSOR(dynamic_attr) {
		static void init(CoreType & r, dynamic_attr && a) { if (!r.tp_dict) { r.tp_dict = DICT(CoreDict{}); } }
	};

	// is_final
	DECL_BASIC_ATTRIBUTE_E(is_final);
	template <> DECL_BASIC_PROCESSOR(is_final) {
		static void init(CoreType & r, is_final && a) { /*r.tp_is_final = true;*/ }
	};

	// metaclass
	DECL_BASIC_ATTRIBUTE_O(metaclass, value);
	template <> DECL_BASIC_PROCESSOR(metaclass) {
		static void init(CoreType & r, type && a) { /*r.set_type(super_cast<CoreType>(a.value));*/ }
	};

	// module_local
	DECL_BASIC_ATTRIBUTE_B(module_local, value);
	template <> DECL_BASIC_PROCESSOR(module_local) {
		static void init(CoreType & r, module_local && a) { /*r.tp_module_local = a.value;*/ }
	};

	// arithmetic
	DECL_BASIC_ATTRIBUTE_E(arithmetic);
	template <> DECL_BASIC_PROCESSOR(arithmetic) {
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

	template <class ... Ts> DECL_BASIC_PROCESSOR(call_guard<Ts...>) {};

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
	> using is_call_guard = mpl::is_instantiation<call_guard, T>;

	template <class ... Extra
	> using extract_guard_t = typename mpl::exactly_one_t<is_call_guard, call_guard<>, Extra...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_DETAIL_ATTR_HPP_
