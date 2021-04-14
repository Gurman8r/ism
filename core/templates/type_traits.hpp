#ifndef _ISM_TYPE_TRAITS_HPP_
#define _ISM_TYPE_TRAITS_HPP_

#include <core/typedefs.hpp>

#include <type_traits>

#define FWD(x) _STD forward<decltype(x)>(x) // automatic forward

// traits
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From
	> constexpr bool is_trivially_convertible_v
	{
		"requires To is trivially default constructible and is copy or move constructible"
		&& sizeof(To) == sizeof(From)
		&& std::is_trivially_copyable_v<From>
		&& std::is_trivial_v<To>
		&& (std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>)
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...> struct void_t_impl { using type = void; };

	template <class ... Ts> using void_t = typename void_t_impl<Ts...>::type;

	struct void_type {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts> ALIAS(all_of) std::conjunction<Ts...>;
	template <class ... Ts> ALIAS(any_of) std::disjunction<Ts...>;
	template <class ... Ts> ALIAS(none_of) std::negation<any_of<Ts...>>;
	template <class ... Ts> constexpr bool all_of_v{ all_of<Ts...>::value };
	template <class ... Ts> constexpr bool any_of_v{ any_of<Ts...>::value };
	template <class ... Ts> constexpr bool none_of_v{ none_of<Ts...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, template<class> class ... Pr> ALIAS(satisfies_all_of) all_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> ALIAS(satisfies_any_of) any_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> ALIAS(satisfies_none_of) none_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> constexpr bool satisfies_all_of_v{ satisfies_all_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_any_of_v{ satisfies_any_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_none_of_v{ satisfies_none_of<T, Pr...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// remove_class
	template <class T> struct remove_class {};
	template <class C, class R, class ... A> struct remove_class<R(C:: *)(A...)> { using type = R(A...); };
	template <class C, class R, class ... A> struct remove_class<R(C:: *)(A...) const> { using type = R(A...); };
	template <class ... Ts> ALIAS(remove_class_t) typename remove_class<Ts...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// intrinsic_type
	template <class T> struct intrinsic_type { using type = T; };
	template <class T> struct intrinsic_type<const T> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T *> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T &> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T &&> { using type = typename intrinsic_type<T>::type; };
	template <class T, size_t N> struct intrinsic_type<T const [N]> { using type = typename intrinsic_type<T>::type; };
	template <class T, size_t N> struct intrinsic_type<T[N]> { using type = typename intrinsic_type<T>::type; };
	template <class T> ALIAS(intrinsic_t) typename intrinsic_type<T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __cpp_fold_expressions
	template <class ... Ts> constexpr size_t constexpr_sum(Ts... ns) { return (0 + ... + size_t{ ns }); }
#else
	constexpr size_t constexpr_sum() { return 0; }
	template <class T, class ... Ts>
	constexpr size_t constexpr_sum(T n, Ts... ns) { return size_t{ n } + constexpr_sum(ns...); }
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t constexpr_impl_first(int32_t i) { return i; }
	
	template <class T, class... Ts
	> constexpr int32_t constexpr_impl_first(int32_t i, T v, Ts... vs) { return v ? i : constexpr_impl_first(i + 1, vs...); }

	constexpr int32_t constexpr_impl_last(int32_t /*i*/, int32_t result) { return result; }
	
	template <class T, class... Ts
	> constexpr int32_t constexpr_impl_last(int32_t i, int32_t result, T v, Ts... vs) { return constexpr_impl_last(i + 1, v ? i : result, vs...); }

	template <template<class> class Pr, class... Ts
	> constexpr int32_t constexpr_first() { return constexpr_impl_first(0, Pr<Ts>::value...); }

	// Return the index of the last type in Ts which satisfies Pr<T>, or -1 if None match.
	template <template<class> class Pr, class... Ts
	> constexpr int32_t constexpr_last() { return constexpr_impl_last(0, -1, Pr<Ts>::value...); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T, class ... Ts
	> struct pack_element { using type = typename pack_element<N - 1, Ts...>::type; };

	template <class T, class ... Ts
	> struct pack_element<0, T, Ts...> { using type = T; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template<class> class Pr, class Default, class ... Ts
	> struct exactly_one
	{
		static constexpr auto found = constexpr_sum(Pr<Ts>::value...);
		static_assert(found <= 1, "Found more than one type matching the predicate");

		static constexpr auto index = found ? constexpr_first<Pr, Ts...>() : 0;
		using type = std::conditional_t<found, class pack_element<index, Ts...>::type, Default>;
	};

	template <template<class> class P, class Default
	> struct exactly_one<P, Default> { using type = Default; };

	template <template<class> class Pr, class Default, class ... Ts
	> using exactly_one_t = typename exactly_one<Pr, Default, Ts...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class... /*Us*/> struct deferred_type { using type = T; };

	template <class T, class... Us> using deferred_t = typename deferred_type<T, Us...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Base, class Derived
	> using is_strict_base_of = std::bool_constant<std::is_base_of_v<Base, Derived> && !std::is_same_v<Base, Derived>>;

	template <class Base, class Derived
	> constexpr bool is_strict_base_of_v{ is_strict_base_of_v<Base, Derived> };
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Base, class Derived
	> using is_accessible_base_of = std::bool_constant<(std::is_same_v<Base, Derived> || std::is_base_of_v<Base, Derived>) && std::is_convertible_v<Derived *, Base *>>;

	template <class Base, class Derived
	> constexpr bool is_accessible_base_of_v{ is_accessible_base_of<Base, Derived>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template<class...> class Base
	> struct is_template_base_of_impl {
		template <class... Us> static std::true_type check(Base<Us...> *);
		static std::false_type check(...);
	};

	template <template<class...> class Base, class T>
#if !ISM_CC_MSVC
	using is_template_base_of = decltype(is_template_base_of_impl<Base>::check((intrinsic_t<T> *)nullptr));
#else
	struct is_template_base_of : decltype(is_template_base_of_impl<Base>::check((intrinsic_t<T> *)nullptr)) {};
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template<class...> class C, class T
	> struct is_instantiation : std::false_type {};

	template <template<class...> class C, class... Us
	> struct is_instantiation<C, C<Us...>> : std::true_type {};

	template <template<class...> class C, class... Us
	> constexpr bool is_instantiation_v{ is_instantiation<C, Us...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = void> struct is_input_iterator : std::false_type {};

	template <class T> struct is_input_iterator<T, void_t<decltype(*std::declval<T &>()), decltype(++std::declval<T &>())>> : std::true_type {};

	template <class T> constexpr bool is_input_iterator_v{ is_input_iterator<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> using is_function_pointer = std::bool_constant<
		std::is_pointer_v<T> &&
		std::is_function_v<typename std::remove_pointer_t<T>>>;

	template <class T> constexpr bool is_function_pointer_v{ is_function_pointer<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class F> struct strip_function_object { using type = typename remove_class_t<decltype(&F::operator())>; };

	template <class F> using strip_function_object_t = typename strip_function_object<F>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Func, class F = std::remove_reference_t<Func>
	> using function_signature_t = std::conditional_t<
		std::is_function_v<F>,
		F,
		typename std::conditional_t<
		std::is_pointer_v<F> || std::is_member_pointer_v<F>,
		std::remove_pointer<F>,
		strip_function_object<F>
		>::type>;

	template <class T> using is_lambda = satisfies_none_of<std::remove_reference_t<T>,
		std::is_function,
		std::is_pointer,
		std::is_member_pointer>;

	template <class T> constexpr bool is_lambda_v{ is_lambda<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __cpp_fold_expressions
#define ISM_EXPAND_SIDE_EFFECTS(PATTERN) (((PATTERN), void()), ...)
#else
	using expand_side_effects = bool[];
#define ISM_EXPAND_SIDE_EFFECTS(PATTERN) (void)ism::expand_side_effects{ ((PATTERN), void(), false)..., false }
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class itype, class SFINAE = void
	> struct polymorphic_type_hook_base
	{
		static void const * get(itype const * src, std::type_info const *&) { return src; }
	};

	template <class itype
	> struct polymorphic_type_hook_base<itype, std::enable_if_t<std::is_polymorphic_v<itype>>>
	{
		static void const * get(itype const * src, std::type_info const *& type) {
			type = src ? &typeid(*src) : nullptr;
			return dynamic_cast<void const *>(src);
		}
	};

	template <class itype, class SFINAE = void
	> struct polymorphic_type_hook : public polymorphic_type_hook_base<itype> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts> struct call_guard;

	template <class ... Ts> using call_guard_t = typename call_guard<Ts...>::type;

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
			T guard{};
			call_guard_t<Ts...> next{};
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct has_operator_delete : std::false_type {};

	template <class T, class SFINAE = void> struct has_operator_delete_size : std::false_type {};
	
	template <class T> struct has_operator_delete<T, void_t<decltype(static_cast<void (*)(void *)>(T::operator delete))>> : std::true_type {};

	template <class T> struct has_operator_delete_size<T, void_t<decltype(static_cast<void (*)(void *, size_t)>(T::operator delete))>> : std::true_type {};
	
	template <class T> constexpr bool has_operator_delete_v{ has_operator_delete<T>::value };
	
	template <class T> constexpr bool has_operator_delete_size_v{ has_operator_delete_size<T>::value };

	// Call class-specific delete if it exists or global otherwise. Can also be an overload set.
	template <class T> void call_operator_delete(T * p, size_t s, size_t a)
	{
		if constexpr (has_operator_delete_v<T>)
		{
			T::operator delete(p);
		}
		else if constexpr (has_operator_delete_size_v<T>)
		{
			T::operator delete(p, s);
		}
		else
		{
			(void)s; (void)a;
#if defined(__cpp_aligned_new) && (!defined(_MSC_VER) || _MSC_VER >= 1912)
			if (a > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
				::operator delete(p, s, std::align_val_t{ a });
#else
				::operator delete(p, std::align_val_t{ a });
#endif
				return;
			}
#endif
#ifdef __cpp_sized_deallocation
			::operator delete(p, s);
#else
			::operator delete(p);
#endif
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// operators
namespace ism
{
	template <class _Ty> struct Plus
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs + rhs;
		}
	};

	template <class _Ty> struct Minus
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs - rhs;
		}
	};

	template <class _Ty> struct Multiplies
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs * rhs;
		}
	};

	template <class _Ty> struct EqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs == rhs;
		}
	};

	template <class _Ty> struct NotEqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs != rhs;
		}
	};

	template <class _Ty> struct Less
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs < rhs;
		}
	};

	template <class _Ty> struct Greater
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs > rhs;
		}
	};

	template <class _Ty> struct LessEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs <= rhs;
		}
	};

	template <class _Ty> struct GreaterEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const {
			return lhs >= rhs;
		}
	};
}

#endif // !_ISM_TYPE_TRAITS_HPP_
