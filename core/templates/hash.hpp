#ifndef _ISM_HASH_HPP_
#define _ISM_HASH_HPP_

#include <core/templates/utility.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD FNV1A final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr FNV1A() noexcept = default;

		static constexpr hash_t basis{ static_cast<hash_t>(14695981039346656037ULL) };

		static constexpr hash_t prime{ static_cast<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> NODISCARD constexpr hash_t operator()(T const * arr, hash_t size, hash_t seed) const
		{
			return size
				? FNV1A{}(arr + 1, size - 1, (seed ^ static_cast<hash_t>(*arr)) * prime)
				: seed;
		}

		template <class T
		> NODISCARD constexpr hash_t operator()(T const * arr, hash_t size) const
		{
			return FNV1A{}(arr, size, basis);
		}

		template <class T, hash_t N
		> NODISCARD constexpr hash_t operator()(T const(&value)[N]) const
		{
			return FNV1A{}(value, N - 1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class = std::enable_if_t<std::is_scalar_v<T> && !std::is_pointer_v<T>>
		> NODISCARD constexpr hash_t operator()(T const value) const
		{
			return (basis ^ static_cast<hash_t>(value)) * prime;
		}

		template <class Arr, class = std::enable_if_t<std::is_object_v<Arr>>
		> NODISCARD constexpr hash_t operator()(Arr const & value) const
		{
			return FNV1A{}(value.data(), static_cast<hash_t>(value.size()));
		}

		template <template <class, hash_t...> class Arr, class T, hash_t ... N
		> NODISCARD constexpr hash_t operator()(Arr<T, N...> const & value) const
		{
			return FNV1A{}(value.data(), static_cast<hash_t>(value.size()));
		}

		template <template <class...> class Arr, class ... Ts
		> NODISCARD constexpr hash_t operator()(Arr<Ts...> const & value) const
		{
			return FNV1A{}(value.data(), static_cast<hash_t>(value.size()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class H = FNV1A, class Arg0, class ... Args
	> NODISCARD constexpr hash_t hash(Arg0 && arg0, Args && ... args)
	{
		return H{}(FWD(arg0), FWD(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD constexpr hash_t operator ""_hash(cstring str, size_t const len)
	{
		return hash(str, len);
	}

	NODISCARD constexpr hash_t operator ""_hash(cwstring str, size_t const len)
	{
		return hash(str, len);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher {
		Hasher() = default;
		NODISCARD hash_t operator()(T const & value) const {
			return std::hash<T>{}(value);
		}
	};

	template <> struct Hasher<float32_t> {
		Hasher() = default;
		NODISCARD hash_t operator()(float32_t const value) const {
			return std::hash<float32_t>{}(value);
		}
	};

	template <> struct Hasher<float64_t> {
		Hasher() = default;
		NODISCARD hash_t operator()(float64_t const value) const {
			return std::hash<float64_t>{}(value);
		}
	};

	template <> struct Hasher<float80_t> {
		Hasher() = default;
		NODISCARD hash_t operator()(float80_t const value) const {
			return std::hash<float80_t>{}(value);
		}
	};

	template <> struct Hasher<nullptr_t> {
		Hasher() = default;
		NODISCARD hash_t operator()(nullptr_t) const {
			return std::hash<nullptr_t>{}(nullptr_t{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_HPP_
