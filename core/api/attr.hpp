#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/api/call.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::attr
{
	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		using type = typename T;
		static void init(T &&, FunctionRecord &) {}
		static void init(T &&, TypeObject &) {}
		static void precall(FunctionCall &) {}
		static void postcall(FunctionCall &, OBJECT) {}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// process attribute default
#define ISM_PROCESS_ATTR(m_class) \
	struct ism::attr::process_attribute<m_class> : ism::attr::process_attribute_default<m_class>

// process attribute base
#define ISM_PROCESS_ATTR_BASE(m_class, m_inherits) \
	struct ism::attr::process_attribute<m_class> : ism::attr::process_attribute_default<m_inherits>

// process attribute sfinae
#define ISM_PROCESS_ATTR_SFINAE(m_class, m_sfinae) \
	struct ism::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::attr::process_attribute_default<m_class>

// process attribute base/sfinae
#define ISM_PROCESS_ATTR_BASE_SFINAE(m_class, m_sfinae, m_inherits) \
	struct ism::attr::process_attribute<m_class, std::enable_if_t<m_sfinae>> : ism::attr::process_attribute_default<m_inherits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// empty attribute
#define ISM_ATTR_EMPTY(m_class) \
	struct m_class final { \
		explicit m_class() noexcept {} \
	}

// object attribute
#define ISM_ATTR_OBJECT(m_class, m_value) \
	struct m_class final { \
		ism::BaseObject * const m_value; \
		explicit m_class(ism::BaseObject * value) : m_value{ value } {} \
		explicit m_class(ism::OBJECT value) : m_value{ *value } {} \
	}

// string attribute
#define ISM_ATTR_STRING(m_class, m_value) \
	struct m_class final { \
		char const * const m_value; \
		explicit m_class(char const * value) : m_value{ value } {} \
		explicit m_class(char * value) : m_value{ (char const *)value } {} \
	}

// bool attribute
#define ISM_ATTR_BOOL(m_class, m_value) \
	struct m_class final { \
		bool const m_value; \
		explicit m_class(bool value) : m_value{ value } {} \
	}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	ISM_ATTR_STRING(name, value);
	template <> ISM_PROCESS_ATTR(name) {
		static void init(FunctionRecord & r, name && a) { r.name = a.value; }
		static void init(TypeObject & r, name && a) { r.tp_name = a.value; }
	};

	// doc
	ISM_ATTR_STRING(doc, value);
	template <> ISM_PROCESS_ATTR(doc) {
		static void init(FunctionRecord & r, doc && a) { r.doc = a.value; }
		static void init(TypeObject & r, doc && a) { r.tp_doc = a.value; }
	};
	template <> ISM_PROCESS_ATTR(cstring) {
		static void init(FunctionRecord & r, cstring a) { r.doc = a; }
		static void init(TypeObject & r, cstring a) { r.tp_doc = a; }
	};
	template <> ISM_PROCESS_ATTR_BASE(char *, cstring) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ReturnPolicy
	template <> ISM_PROCESS_ATTR(ReturnPolicy) {
		static void init(FunctionRecord & r, ReturnPolicy && a) { r.policy = a; }
	};

	// sibling
	ISM_ATTR_OBJECT(sibling, value);
	template <> ISM_PROCESS_ATTR(sibling) {
		static void init(FunctionRecord & r, sibling && a) { r.sibling = a.value; }
	};

	// is_method
	ISM_ATTR_OBJECT(is_method, value);
	template <> ISM_PROCESS_ATTR(is_method) {
		static void init(FunctionRecord & r, is_method && a) { r.is_method = true; r.scope = a.value; }
	};

	// scope
	ISM_ATTR_OBJECT(scope, value);
	template <> ISM_PROCESS_ATTR(scope) {
		static void init(FunctionRecord & r, scope && a) { r.scope = a.value; }
	};

	// is_operator
	ISM_ATTR_EMPTY(is_operator);
	template <> ISM_PROCESS_ATTR(is_operator) {
		static void init(FunctionRecord & r, is_operator && a) { r.is_operator = true; }
	};

	// is_constructor
	ISM_ATTR_EMPTY(is_constructor);
	template <> ISM_PROCESS_ATTR(is_constructor) {
		static void init(FunctionRecord & r, is_constructor && a) { r.is_constructor = true; }
	};

	// prepend
	ISM_ATTR_EMPTY(prepend);
	template <> ISM_PROCESS_ATTR(prepend) {
		static void init(FunctionRecord & r, prepend && a) { r.prepend = true; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// parent class
	template <class T> ISM_PROCESS_ATTR_SFINAE(T, is_object_api_v<T>) {
		static void init(TypeObject & r, T && a) { LIST(r.tp_bases).append(typeof(FWD(a))); }
	};

	// multiple inheritance
	ISM_ATTR_EMPTY(multiple_inheritance);
	template <> ISM_PROCESS_ATTR(multiple_inheritance) {
		static void init(TypeObject & r, multiple_inheritance && a) { if (!r.tp_bases) { r.tp_bases = LIST::new_(); } }
	};

	// dynamic_attr
	ISM_ATTR_EMPTY(dynamic_attr);
	template <> ISM_PROCESS_ATTR(dynamic_attr) {
		static void init(TypeObject & r, dynamic_attr && a) { if (!r.tp_dict) { r.tp_dict = DICT::new_(); } }
	};

	// is_final
	ISM_ATTR_EMPTY(is_final);
	template <> ISM_PROCESS_ATTR(is_final) {
		static void init(TypeObject & r, is_final && a) { /*r.tp_is_final = true;*/ }
	};

	// metaclass
	ISM_ATTR_OBJECT(metaclass, value);
	template <> ISM_PROCESS_ATTR(metaclass) {
		static void init(TypeObject & r, type && a) { /*r.set_type(super_cast<TypeObject>(a.value));*/ }
	};

	// module_local
	ISM_ATTR_BOOL(module_local, value);
	template <> ISM_PROCESS_ATTR(module_local) {
		static void init(TypeObject & r, module_local && a) { /*r.tp_module_local = a.value;*/ }
	};

	// arithmetic
	ISM_ATTR_EMPTY(arithmetic);
	template <> ISM_PROCESS_ATTR(arithmetic) {
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
			T guard{}; // compose multiple guard types with left-to-right default-constructor order

			typename call_guard<Ts...>::type next{};
		};
	};

	template <class ... Ts> ISM_PROCESS_ATTR(call_guard<Ts...>) {};

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

		static void postcall(FunctionCall & call, OBJECT retv)
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
