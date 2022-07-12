#ifndef _ISM_TYPE_TRAITS_HPP_
#define _ISM_TYPE_TRAITS_HPP_

#include <core/typedefs.hpp>

#include <type_traits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// forward
#define FWD(expr) \
		(std::forward<decltype(expr)>(expr))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism::priv
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// maker helper
	template <class T> struct _MakerHelper final
	{
		T value;

		template <class ... Args
		> constexpr _MakerHelper(Args && ... args) noexcept : value{ FWD(args)... } {}

		template <class Fn = void(*)(T &)
		> constexpr decltype(auto) operator+(Fn fn) && noexcept
		{
			return fn(value), std::move(value);
		}
	};

#define MAKER(m_class, ...) \
		(ism::priv::_MakerHelper<m_class>{ ##__VA_ARGS__ })

#define MAKE(m_class, m_var, ...) \
		MAKER(m_class, ##__VA_ARGS__) + [&](m_class & m_var) -> void

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// traits
namespace ism::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct void_type {};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Ts
	> constexpr bool is_any_of_v{ std::disjunction_v<std::is_same<T, Ts>...> };

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts> using all_of = std::conjunction<Ts...>;
	template <class ... Ts> using any_of = std::disjunction<Ts...>;
	template <class ... Ts> using none_of = std::negation<any_of<Ts...>>;
	template <class ... Ts> constexpr bool all_of_v{ all_of<Ts...>::value };
	template <class ... Ts> constexpr bool any_of_v{ any_of<Ts...>::value };
	template <class ... Ts> constexpr bool none_of_v{ none_of<Ts...>::value };

	template <class T, template<class> class ... Pr> using satisfies_all_of = all_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> using satisfies_any_of = any_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> using satisfies_none_of = none_of<Pr<T>...>;
	template <class T, template<class> class ... Pr> constexpr bool satisfies_all_of_v{ satisfies_all_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_any_of_v{ satisfies_any_of<T, Pr...>::value };
	template <class T, template<class> class ... Pr> constexpr bool satisfies_none_of_v{ satisfies_none_of<T, Pr...>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> using is_char = std::bool_constant<any_of_v
	<
		std::is_same<C, char>,
#if CXX_20
		std::is_same<C, char8_t>,
#endif
		std::is_same<C, char16_t>,
		std::is_same<C, char32_t>,
		std::is_same<C, wchar_t>
	>>;

	template <class C> constexpr bool is_char_v{ is_char<C>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> using is_number = std::bool_constant<!is_char_v<T> && (std::is_arithmetic_v<T> || std::is_enum_v<T>)>;

	template <class T> constexpr bool is_number_v{ is_number<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class> constexpr bool is_string_v{ false };

	template <class C
	> constexpr bool is_string_v<C *>{ is_char_v<C> };

	template <class C
	> constexpr bool is_string_v<C const *>{ is_char_v<C> };

	template <class C, size_t N
	> constexpr bool is_string_v<C[N]>{ is_char_v<C> };

	template <class C, size_t N
	> constexpr bool is_string_v<C(&)[N]>{ is_char_v<C> };

	template <class C, size_t N
	> constexpr bool is_string_v<C const (&)[N]>{ is_char_v<C> };

	template <class C, size_t N
	> constexpr bool is_string_v<C const [N]>{ is_char_v<C> };

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

	constexpr i32 constexpr_impl_first(i32 i) { return i; }
	
	template <class T, class... Ts
	> constexpr i32 constexpr_impl_first(i32 i, T v, Ts... vs) { return v ? i : constexpr_impl_first(i + 1, vs...); }

	constexpr i32 constexpr_impl_last(i32 /*i*/, i32 result) { return result; }
	
	template <class T, class... Ts
	> constexpr i32 constexpr_impl_last(i32 i, i32 result, T v, Ts... vs) { return constexpr_impl_last(i + 1, v ? i : result, vs...); }

	template <template<class> class Pr, class... Ts
	> constexpr i32 constexpr_first() { return constexpr_impl_first(0, Pr<Ts>::value...); }

	// Return the index of the last type in Ts which satisfies Pr<T>, or -1 if None match.
	template <template<class> class Pr, class... Ts
	> constexpr i32 constexpr_last() { return constexpr_impl_last(0, -1, Pr<Ts>::value...); }

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
#if !CC_MSVC
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// hash
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FNV1A final
	{
		constexpr FNV1A() noexcept = default;

		static constexpr hash_t fnv1a_basis{ static_cast<hash_t>(14695981039346656037ULL) };

		static constexpr hash_t fnv1a_prime{ static_cast<hash_t>(1099511628211ULL) };

		template <class T
		> constexpr hash_t operator()(T const * data, hash_t size, hash_t seed = fnv1a_basis) const noexcept
		{
			return size
				? operator()(data + 1, size - 1, (seed ^ static_cast<hash_t>(*data)) * fnv1a_prime)
				: seed;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class H = FNV1A, class T
	> constexpr hash_t hash(T const * data, size_t size) { return H{}(data, size); }

	constexpr hash_t operator ""_hash(cstring data, size_t size) { return ism::hash(data, size); }

	constexpr hash_t operator ""_hash(cwstring data, size_t size) { return ism::hash(data, size); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher {
		constexpr Hasher() noexcept = default;
	};

	template <> struct Hasher<sbyte> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(sbyte value) const { return std::hash<sbyte>{}(value); }
	};

	template <> struct Hasher<i16> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(i16 value) const { return std::hash<i16>{}(value); }
	};

	template <> struct Hasher<i32> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(i32 value) const { return std::hash<i32>{}(value); }
	};

	template <> struct Hasher<i64> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(i64 value) const { return std::hash<i64>{}(value); }
	};

	template <> struct Hasher<byte> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(byte value) const { return std::hash<byte>{}(value); }
	};

	template <> struct Hasher<u16> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(u16 value) const { return std::hash<u16>{}(value); }
	};

	template <> struct Hasher<u32> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(u32 value) const { return std::hash<u32>{}(value); }
	};

	template <> struct Hasher<u64> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(u64 value) const { return std::hash<u64>{}(value); }
	};

	template <> struct Hasher<f32> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(f32 value) const { return std::hash<f32>{}(value); }
	};

	template <> struct Hasher<f64> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(f64 value) const { return std::hash<f64>{}(value); }
	};

	template <> struct Hasher<f80> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(f80 value) const { return std::hash<f80>{}(value); }
	};

	template <> struct Hasher<nullptr_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(nullptr_t) const { return std::hash<nullptr_t>{}(nullptr_t{}); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_TYPE_TRAITS_HPP_
