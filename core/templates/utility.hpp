#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/os/copymem.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <type_traits>
#include <tuple>
#include <variant>

#include <gcem/include/gcem.hpp>

namespace ism
{
	// pair
	template <class First, class Second
	> ALIAS(Pair) std::pair<First, Second>;

	// tuple
	template <class First, class ... Rest
	> ALIAS(Tuple) std::tuple<First, Rest...>;
}

// compose
namespace ism::impl
{
	template <class T> struct ComposeHelper final
	{
		T value;

		constexpr ComposeHelper(T && value) noexcept : value{ FWD(value) } {}

		template <class Fn = void(*)(T &)
		> constexpr decltype(auto) operator+(Fn && fn) && noexcept
		{
			return fn(value), std::move(value);
		}
	};

#define COMPOSE_EX(m_class, ...) \
		(ism::impl::ComposeHelper<m_class>(m_class{ ##__VA_ARGS__ }))

#define COMPOSE(m_class, m_var, ...) \
		COMPOSE_EX(m_class, ##__VA_ARGS__) + [&](m_class & m_var) -> void
}

// operator structures
namespace ism
{
	template <class _Ty> struct Plus
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs + rhs; }
	};

	template <class _Ty> struct Minus
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs - rhs; }
	};

	template <class _Ty> struct Multiplies
	{
		using result_type = typename _Ty;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr _Ty operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs * rhs; }
	};

	template <class _Ty> struct EqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs == rhs; }
	};

	template <class _Ty> struct NotEqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs != rhs; }
	};

	template <class _Ty> struct Less
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs < rhs; }
	};

	template <class _Ty> struct Greater
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs > rhs; }
	};

	template <class _Ty> struct LessEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs <= rhs; }
	};

	template <class _Ty> struct GreaterEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename _Ty;
		using second_argument_type = typename _Ty;
		NODISCARD constexpr bool operator()(_Ty const & lhs, _Ty const & rhs) const { return lhs >= rhs; }
	};
}

// utility
namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Ts
	> constexpr bool is_any_of_v
	{
		std::disjunction_v<std::is_same<T, Ts>...>
	};

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

	template <class To, class From, class = std::enable_if_t<is_trivially_convertible_v<To, From>>
	> NODISCARD To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	}

	template <class A, class B
	> constexpr int32_t compare(A const & a, B const & b) noexcept
	{
		return CMP(a, b);
	}

	template <class LI, class RI
	> NODISCARD constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && util::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	template <class LI, class RI
	> NODISCARD constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && util::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// traits
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...> struct void_t_impl { using type = void; };

	template <class ... Ts> using void_t = typename void_t_impl<Ts...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts> using all_of = std::conjunction<Ts...>;
	template <class ... Ts> using any_of = std::disjunction<Ts...>;
	template <class ... Ts> using none_of = std::negation<any_of<Ts...>>;
	template <class ... Ts> constexpr bool all_of_v{ all_of<Ts...>::value };
	template <class ... Ts> constexpr bool any_of_v{ any_of<Ts...>::value };
	template <class ... Ts> constexpr bool none_of_v{ none_of<Ts...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, template<class> class ... Pr> using satisfies_all_of = all_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> using satisfies_any_of = any_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> using satisfies_none_of = none_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> constexpr bool satisfies_all_of_v{ satisfies_all_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_any_of_v{ satisfies_any_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_none_of_v{ satisfies_none_of<T, Pr...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch> constexpr bool is_char_v{ any_of_v<
		std::is_same<Ch, char>,
#if ISM_CXX_20
		std::is_same<Ch, char8_t>,
#endif
		std::is_same<Ch, char16_t>,
		std::is_same<Ch, char32_t>,
		std::is_same<Ch, wchar_t>
	> };

	template <class T> constexpr bool is_number_v{ (std::is_arithmetic_v<T> || std::is_enum_v<T>) && !mpl::is_char_v<T> };

	template <class> constexpr bool is_string_v{ false };

	template <class Ch
	> constexpr bool is_string_v<Ch *>{ is_char_v<Ch> };

	template <class Ch
	> constexpr bool is_string_v<Ch const *>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch[N]>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch(&)[N]>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch const (&)[N]>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch const [N]>{ is_char_v<Ch> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct intrinsic_type { using type = T; };
	template <class T> struct intrinsic_type<const T> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T *> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T &> { using type = typename intrinsic_type<T>::type; };
	template <class T> struct intrinsic_type<T &&> { using type = typename intrinsic_type<T>::type; };
	template <class T, size_t N> struct intrinsic_type<T const [N]> { using type = typename intrinsic_type<T>::type; };
	template <class T, size_t N> struct intrinsic_type<T[N]> { using type = typename intrinsic_type<T>::type; };
	template <class T> using intrinsic_t = typename intrinsic_type<T>::type;

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
#if !COMPILER_MSVC
	using is_template_base_of = decltype(is_template_base_of_impl<Base>::check((intrinsic_t<T> *)nullptr));
#else
	struct is_template_base_of : decltype(is_template_base_of_impl<Base>::check((intrinsic_t<T> *)nullptr)) {};
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template<class...> class C, class T
	> struct is_instantiation_t : std::false_type {};

	template <template<class...> class C, class... Us
	> struct is_instantiation_t<C, C<Us...>> : std::true_type {};

	template <template<class...> class C, class... Us
	> constexpr bool is_instantiation_v{ is_instantiation_t<C, Us...>::value };

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

	template <class T> struct remove_class {};
	template <class C, class R, class ... A> struct remove_class<R(C:: *)(A...)> { using type = R(A...); };
	template <class C, class R, class ... A> struct remove_class<R(C:: *)(A...) const> { using type = R(A...); };
	template <class ... Ts> using remove_class_t = typename remove_class<Ts...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class F> struct strip_function_object { using type = remove_class_t<decltype(&F::operator())>; };

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
			UNUSED(s); UNUSED(a);
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

	template<class
	> struct is_ref_wrapper : std::false_type {};

	template <class T
	> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

	template <class T
	> using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

	template <class D, class...
	> struct return_type_helper { using type = D; };

	template <class... Args
	> struct return_type_helper<void, Args...> : std::common_type<Args...>
	{
		static_assert(
			std::conjunction_v<not_ref_wrapper<Args>...>,
			"Args cannot contain reference_wrappers when D is void");
	};

	template <class ... Types
	> using return_type_t = typename return_type_helper<Types...>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void
	> struct is_copy_constructible : std::is_copy_constructible<T> {};

	template <class Container
	> struct is_copy_constructible<Container, std::enable_if_t<mpl::all_of_v<
		std::is_copy_constructible<Container>,
		std::is_same<typename Container::value_type &,
		typename Container::reference>,
		std::negation<std::is_same<Container, typename Container::value_type>>
		>>> : is_copy_constructible<typename Container::value_type> {};

	template <class T1, class T2
	> struct is_copy_constructible<Pair<T1, T2>> : mpl::all_of<is_copy_constructible<T1>, is_copy_constructible<T2>> {};

	template <class T> constexpr bool is_copy_constructible_v{ is_copy_constructible<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void
	> struct is_copy_assignable : std::is_copy_assignable<T> {};

	template <class Container
	> struct is_copy_assignable<Container, std::enable_if_t<mpl::all_of_v<
		std::is_copy_assignable<Container>,
		std::is_same<typename Container::value_type &, typename Container::reference>
		>>> : is_copy_assignable<typename Container::value_type> {};

	template <class T1, class T2
	> struct is_copy_assignable<Pair<T1, T2>> : mpl::all_of<is_copy_assignable<T1>, is_copy_assignable<T2>> {};

	template <class T> constexpr bool is_copy_assignable_v{ is_copy_assignable<T>::value };

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
}

// META STRUCTURES
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type_list
	template <class ... Ts
	> struct type_list { static constexpr size_t size{ sizeof...(Ts) }; };

	// type_tag
	template <class T
	> struct type_tag { using type = typename T; };

#define TAG_TYPE(tag) std::decay_t<decltype(tag)::type>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// LOOPS
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn, size_t ... Is
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn, std::index_sequence<Is...>) noexcept
	{
		fn(std::get<Is>(FWD(tp))...);
	}

	template <class Tup, class Fn
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(
			FWD(tp),
			FWD(fn),
			std::make_index_sequence<std::tuple_size_v<std::decay_t<Tup>>>());
	}

	// "unpacks" the contents of a tuple inside a function call
	template <class Tup, class Fn
	> constexpr void tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(FWD(tp), FWD(fn));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class ... Args
	> constexpr void impl_for_args(Fn && fn, Args && ... args) noexcept
	{
		if constexpr (0 < sizeof...(args)) // sanity check
		{
			(void)std::initializer_list<int>{ (fn(FWD(args)), 0)... };
		}
	}

	// invoke function on every argument
	template <class Fn, class ... Args
	> constexpr void for_args(Fn && fn, Args && ... args) noexcept
	{
		mpl::impl_for_args(FWD(fn), FWD(args)...);
	}

	// invoke function on every argument with index
	template <class Fn, class ... Args
	> constexpr void for_args_i(Fn && fn, Args && ... args) noexcept
	{
		size_t i{};
		mpl::for_args([&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		}
		, FWD(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn
	> constexpr void impl_for_tuple(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(FWD(tp), [&fn](auto && ... rest) noexcept
		{
			mpl::for_args(fn, FWD(rest)...);
		});
	}

	// invoke function on every tuple element
	template <class Tup, class Fn
	> constexpr void for_tuple(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_for_tuple(FWD(tp), FWD(fn));
	}

	// invoke function on every tuple element with index
	template <class Tup, class Fn
	> constexpr void for_tuple_i(Tup && tp, Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_tuple(FWD(tp), [&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONCAT - combine types
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct impl_concat
	{
		using type = typename type_list<>;
	};

	template <class ... Ts
	> using concat = typename impl_concat<Ts...>::type;

	template <class ... Ts
	> struct impl_concat<type_list<Ts...>>
	{
		using type = typename type_list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct impl_concat<type_list<Ts0...>, type_list<Ts1...>, Rest...>
	{
		using type = typename concat<type_list<Ts0..., Ts1...>, Rest...>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REMAP - modify inner types
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct impl_remap
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> using remap = typename impl_remap<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_remap<Pr, type_list<T, Ts...>>
	{
		using type = typename concat<type_list<Pr<T>>, remap<Pr, type_list<Ts...>>>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// RENAME - modify outer type
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class To, class T
	> struct impl_rename;

	template <
		template <class ...> class To,
		template <class...> class From,
		class ... Ts
	> struct impl_rename<To, From<Ts...>>
	{
		using type = typename To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> using rename = typename impl_rename<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONTAINS - check type_list contains type
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct contains;

	template <class T
	> struct contains<T, type_list<>>
		: std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, type_list<U, Ts...>>
		: contains<T, type_list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, type_list<T, Ts...>>
		: std::true_type {};

	template <class T, class Ls
	> constexpr bool contains_v{ contains<T, Ls>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// INDEX OF - get index of type in a type_list
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, type_list<T, Ts...>>
		: std::integral_constant<size_t, 0> {};

	template <class T, class U, class ... Ts
	> struct index_of<T, type_list<U, Ts...>>
		: std::integral_constant<size_t, 1 + index_of<T, type_list<Ts...>>::value> {};

	template <class T, class Ls
	> constexpr auto index_of_v{ index_of<T, Ls>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// GENERAL
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Ls
	> using tuple = rename<std::tuple, Ls>;
	
	template <class Ls
	> using tag_tuple = tuple<remap<type_tag, Ls>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls
	> constexpr size_t size() noexcept { return Ls::size; }

	template <class Ls, class T> using push_back = concat<Ls, type_list<T>>;

	template <class Ls, class T> using push_front = concat<type_list<T>, Ls>;

	template <size_t I, class Ls> using nth = std::tuple_element_t<I, tuple<Ls>>;

	template <class Ls> using head = nth<0, Ls>;

	template <class Ls> using tail = nth<size<Ls>() - 1, Ls>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls, class Fn
	> constexpr void for_type_list(Fn && fn) noexcept
	{
		if constexpr (0 < mpl::size<Ls>()) // sanity check
		{
			mpl::for_tuple(mpl::tag_tuple<Ls>{}, fn);
		}
	}

	template <class Ls, class Fn
	> constexpr void for_type_list_i(Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_type_list<Ls>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts, class Fn
	> constexpr void for_types(Fn && fn) noexcept
	{
		mpl::for_type_list<mpl::type_list<Ts...>>(FWD(fn));
	}

	template <class ... Ts, class Fn
	> constexpr void for_types_i(Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_types<Ts...>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REPEAT - size N type_list of T
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T
	> struct impl_repeat
	{
		using type = typename push_back<typename impl_repeat<N - 1, T>::type, T>;
	};

	template <class T
	> struct impl_repeat<0, T>
	{
		using type = typename type_list<>;
	};

	template <size_t N, class T
	> using repeat = typename impl_repeat<N, T>::type;

	template <class T, size_t N
	> using array = tuple<repeat<N, T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// FILTER - remove types from type_list
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct impl_filter
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> using filter = typename impl_filter<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_filter<Pr, type_list<T, Ts...>>
	{
		using next = typename filter<Pr, type_list<Ts...>>;

		using type = typename std::conditional_t<
			(Pr<T>{}),
			concat<type_list<T>, next>,
			next
		>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_UTILITY_HPP_