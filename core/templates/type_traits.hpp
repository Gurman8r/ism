#ifndef _ISM_TYPE_TRAITS_HPP_
#define _ISM_TYPE_TRAITS_HPP_

#include <core/typedefs.hpp>

#include <type_traits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// forward
#define FWD(expr) \
		(std::forward<decltype(expr)>(expr))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// traits
namespace ism::mpl
{
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
#if CXX_20
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

	constexpr hash_t operator ""_hash(cstring data, size_t size) { return hash(data, size); }

	constexpr hash_t operator ""_hash(cwstring data, size_t size) { return hash(data, size); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher {
		constexpr Hasher() noexcept = default;
	};

	template <> struct Hasher<int8_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(int8_t value) const { return std::hash<int8_t>{}(value); }
	};

	template <> struct Hasher<int16_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(int16_t value) const { return std::hash<int16_t>{}(value); }
	};

	template <> struct Hasher<int32_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(int32_t value) const { return std::hash<int32_t>{}(value); }
	};

	template <> struct Hasher<int64_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(int64_t value) const { return std::hash<int64_t>{}(value); }
	};

	template <> struct Hasher<uint8_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(uint8_t value) const { return std::hash<uint8_t>{}(value); }
	};

	template <> struct Hasher<uint16_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(uint16_t value) const { return std::hash<uint16_t>{}(value); }
	};

	template <> struct Hasher<uint32_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(uint32_t value) const { return std::hash<uint32_t>{}(value); }
	};

	template <> struct Hasher<uint64_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(uint64_t value) const { return std::hash<uint64_t>{}(value); }
	};

	template <> struct Hasher<float32_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(float32_t value) const { return std::hash<float32_t>{}(value); }
	};

	template <> struct Hasher<float64_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(float64_t value) const { return std::hash<float64_t>{}(value); }
	};

	template <> struct Hasher<float80_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(float80_t value) const { return std::hash<float80_t>{}(value); }
	};

	template <> struct Hasher<nullptr_t> {
		constexpr Hasher() noexcept = default;
		hash_t operator()(nullptr_t) const { return std::hash<nullptr_t>{}(nullptr_t{}); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_TYPE_TRAITS_HPP_
