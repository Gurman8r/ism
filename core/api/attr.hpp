#ifndef _ISM_ATTR_HPP_
#define _ISM_ATTR_HPP_

#include <core/api/cast.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _IsMethod { OBJECT class_; };
	
	struct _IsOperator {};
	
	struct _IsFinal {};

	struct _Scope { OBJECT value; };
	
	struct _Doc { cstring value; };
	
	struct _Name { cstring value; };
	
	struct _Sibling { OBJECT value; };

	struct _MultipleInheritance {};
	
	struct _DynamicAttr {};
	
	struct _MetaClass { OBJECT value; };

	struct _ModuleLocal { bool const value; };

	struct _Arithmetic {};

	struct _Prepend {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class... Ts> struct call_guard;

	template <> struct call_guard<> { using type = void_type; };

	template <class T> struct call_guard<T>
	{
		static_assert(std::is_default_constructible<T>::value, "The guard type must be default constructible");

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ArgumentRecord
	{
		String name{};

		OBJECT value{};
		
		bool convert{}, none{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FunctionRecord
	{
		String name{}, doc{}, signature{};

		Vector<ArgumentRecord> args{};

		StdFn<OBJECT(FunctionCall &)> impl{};

		void * data[3]{};

		StdFn<void(FunctionRecord *)> free_data{};

		ReturnPolicy policy{};

		bool is_constructor{}, is_stateless{}, is_operator{}, is_method{}, has_args{}, prepend{};

		uint16_t nargs{};

		MethodDef * def{};

		OBJECT scope{}, sibling{};

		FunctionRecord * next{};
	};

	inline FunctionCall::FunctionCall(FunctionRecord const & f, OBJECT parent) : func{ f }, parent{ parent }
	{
		args.reserve(f.nargs);
		args_convert.reserve(f.nargs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct TypeRecord
	{
		OBJECT scope{};

		String name{}, doc{};

		std::type_info const * cpptype{};

		size_t type_size{}, type_align{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct process_attribute;

	template <class T> struct process_attribute_default
	{
		static void init(T const &, FunctionRecord *) {}
		static void init(T const &, TypeRecord *) {}
		static void precall(FunctionCall &) {}
		static void postcall(FunctionCall &, OBJECT) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct process_attribute<_Doc> : process_attribute_default<_Doc>
	{
		static void init(_Doc const & doc, FunctionRecord * r) {}
	};

	template <> struct process_attribute<_Name> : process_attribute_default<_Name>
	{
		static void init(_Name const & name, FunctionRecord * r) {}
	};

	template <> struct process_attribute<cstring> : process_attribute_default<cstring>
	{
		static void init(cstring d, FunctionRecord * r) { r->doc = const_cast<char *>(d); }
		static void init(cstring d, TypeRecord * r) { r->doc = const_cast<char *>(d); }
	};

	template <> struct process_attribute<char *> : process_attribute<cstring> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> struct process_attributes
	{
		static void init(Args && ... args, FunctionRecord * r)
		{
			int32_t sink[] = { 0, (process_attribute<std::decay_t<Args>>::init(FWD(args), r), 0) ... };
			UNUSED(sink);
		}

		static void init(Args && ... args, TypeRecord * r)
		{
			int32_t sink[] = { 0, (process_attribute<std::decay_t<Args>>::init(FWD(args), r), 0) ... };
			UNUSED(sink);
		}

		static void precall(FunctionCall & call)
		{
			int32_t sink[] = { 0, (process_attribute<std::decay_t<Args>>::precall(call), 0) ... };
			UNUSED(sink);
		}

		static void postcall(FunctionCall & call, OBJECT r)
		{
			int32_t sink[] = { 0, (process_attribute<std::decay_t<Args>>::postcall(r, retv), 0) ... };
			UNUSED(sink);
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
