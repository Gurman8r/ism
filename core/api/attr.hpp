#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/api/call.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::api::attr
{
	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		using type = typename T;
		static void init(T &&, function_record &) {}
		static void init(T &&, TypeObject &) {}
		static void precall(function_call &) {}
		static void postcall(function_call &, OBJECT) {}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_PROCESS_ATTRIBUTE(m_class) \
	struct ism::api::attr::process_attribute<m_class> : ism::api::attr::process_attribute_default<m_class>

#define ISM_PROCESS_ATTRIBUTE_BASE(m_class, m_inherits) \
	struct ism::api::attr::process_attribute<m_class> : ism::api::attr::process_attribute_default<m_inherits>

#define ISM_PROCESS_ATTRIBUTE_SFINAE(m_class, m_sfinae) \
	struct ism::api::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::api::attr::process_attribute_default<m_class>

#define ISM_PROCESS_ATTRIBUTE_SFINAE_BASE(m_class, m_sfinae, m_inherits) \
	struct ism::api::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::api::attr::process_attribute_default<m_inherits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// empty attribute
#define ISM_BASIC_ATTRIBUTE_E(m_class) \
	struct m_class final { \
		explicit m_class() noexcept {} \
	}

// object attribute
#define ISM_BASIC_ATTRIBUTE_O(m_class, m_value) \
	struct m_class final { \
		ism::api::BaseObject * const m_value; \
		explicit m_class(ism::api::BaseObject * value) : m_value{ value } {} \
		explicit m_class(ism::api::OBJECT value) : m_value{ *value } {} \
	}

// string attribute
#define ISM_BASIC_ATTRIBUTE_S(m_class, m_value) \
	struct m_class final { \
		char const * const m_value; \
		explicit m_class(char const * value) : m_value{ value } {} \
		explicit m_class(char * value) : m_value{ (char const *)value } {} \
	}

// bool attribute
#define ISM_BASIC_ATTRIBUTE_B(m_class, m_value) \
	struct m_class final { \
		bool const m_value; \
		explicit m_class(bool value) : m_value{ value } {} \
	}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::api::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	ISM_BASIC_ATTRIBUTE_S(name, value);
	template <> ISM_PROCESS_ATTRIBUTE(name) {
		static void init(function_record & r, name && a) { r.name = a.value; }
		static void init(TypeObject & r, name && a) { r.tp_name = a.value; }
	};

	// doc
	ISM_BASIC_ATTRIBUTE_S(doc, value);
	template <> ISM_PROCESS_ATTRIBUTE(doc) {
		static void init(function_record & r, doc && a) { r.doc = a.value; }
		static void init(TypeObject & r, doc && a) { r.tp_doc = a.value; }
	};
	template <> ISM_PROCESS_ATTRIBUTE(cstring) {
		static void init(function_record & r, cstring a) { r.doc = a; }
		static void init(TypeObject & r, cstring a) { r.tp_doc = a; }
	};
	template <> ISM_PROCESS_ATTRIBUTE_BASE(char *, cstring) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ReturnPolicy
	template <> ISM_PROCESS_ATTRIBUTE(ReturnPolicy) {
		static void init(function_record & r, ReturnPolicy && a) { r.policy = a; }
	};

	// sibling
	ISM_BASIC_ATTRIBUTE_O(sibling, value);
	template <> ISM_PROCESS_ATTRIBUTE(sibling) {
		static void init(function_record & r, sibling && a) { r.sibling = a.value; }
	};

	// is_method
	ISM_BASIC_ATTRIBUTE_O(is_method, value);
	template <> ISM_PROCESS_ATTRIBUTE(is_method) {
		static void init(function_record & r, is_method && a) { r.is_method = true; r.scope = a.value; }
	};

	// scope
	ISM_BASIC_ATTRIBUTE_O(scope, value);
	template <> ISM_PROCESS_ATTRIBUTE(scope) {
		static void init(function_record & r, scope && a) { r.scope = a.value; }
	};

	// is_operator
	ISM_BASIC_ATTRIBUTE_E(is_operator);
	template <> ISM_PROCESS_ATTRIBUTE(is_operator) {
		static void init(function_record & r, is_operator && a) { r.is_operator = true; }
	};

	// is_constructor
	ISM_BASIC_ATTRIBUTE_E(is_constructor);
	template <> ISM_PROCESS_ATTRIBUTE(is_constructor) {
		static void init(function_record & r, is_constructor && a) { r.is_constructor = true; }
	};

	// prepend
	ISM_BASIC_ATTRIBUTE_E(prepend);
	template <> ISM_PROCESS_ATTRIBUTE(prepend) {
		static void init(function_record & r, prepend && a) { r.prepend = true; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// parent class
	template <class T> ISM_PROCESS_ATTRIBUTE_SFINAE(T, is_object_api_v<T>) {
		static void init(TypeObject & r, T && a) { r.bases->append(typeof(FWD(a))); }
	};

	// multiple inheritance
	ISM_BASIC_ATTRIBUTE_E(multiple_inheritance);
	template <> ISM_PROCESS_ATTRIBUTE(multiple_inheritance) {
		static void init(TypeObject & r, multiple_inheritance && a) { if (!r.tp_bases) { r.tp_bases = LIST(ListObject{}); } }
	};

	// dynamic_attr
	ISM_BASIC_ATTRIBUTE_E(dynamic_attr);
	template <> ISM_PROCESS_ATTRIBUTE(dynamic_attr) {
		static void init(TypeObject & r, dynamic_attr && a) { if (!r.tp_dict) { r.tp_dict = DICT(DictObject{}); } }
	};

	// is_final
	ISM_BASIC_ATTRIBUTE_E(is_final);
	template <> ISM_PROCESS_ATTRIBUTE(is_final) {
		static void init(TypeObject & r, is_final && a) { /*r.tp_is_final = true;*/ }
	};

	// metaclass
	ISM_BASIC_ATTRIBUTE_O(metaclass, value);
	template <> ISM_PROCESS_ATTRIBUTE(metaclass) {
		static void init(TypeObject & r, type && a) { /*r.set_type(super_cast<TypeObject>(a.value));*/ }
	};

	// module_local
	ISM_BASIC_ATTRIBUTE_B(module_local, value);
	template <> ISM_PROCESS_ATTRIBUTE(module_local) {
		static void init(TypeObject & r, module_local && a) { /*r.tp_module_local = a.value;*/ }
	};

	// arithmetic
	ISM_BASIC_ATTRIBUTE_E(arithmetic);
	template <> ISM_PROCESS_ATTRIBUTE(arithmetic) {
		static void init(TypeObject & r, arithmetic && a) { /* nothing to do here */ }
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

	template <class ... Ts> ISM_PROCESS_ATTRIBUTE(call_guard<Ts...>) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::api::attr
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

#endif // !_ISM_ATTR_HPP_
