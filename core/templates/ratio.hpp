#ifndef _ISM_RATIO_HPP_
#define _ISM_RATIO_HPP_

#include <core/templates/utility.hpp>
#include <ratio>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <int64_t N, int64_t D = 1
	> ALIAS(Ratio) std::ratio<N, D>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(Atto)		Ratio<1, 1000000000000000000LL>;
	ALIAS(Femto)	Ratio<1, 1000000000000000LL>;
	ALIAS(Pico)		Ratio<1, 1000000000000LL>;
	ALIAS(Nano)		Ratio<1, 1000000000>;
	ALIAS(Micro)	Ratio<1, 1000000>;
	ALIAS(Milli)	Ratio<1, 1000>;
	ALIAS(Centi)	Ratio<1, 100>;
	ALIAS(Deci)		Ratio<1, 10>;
	ALIAS(Deca)		Ratio<10, 1>;
	ALIAS(Hecto)	Ratio<100, 1>;
	ALIAS(Kilo)		Ratio<1000, 1>;
	ALIAS(Mega)		Ratio<1000000, 1>;
	ALIAS(Giga)		Ratio<1000000000, 1>;
	ALIAS(Tera)		Ratio<1000000000000LL, 1>;
	ALIAS(Peta)		Ratio<1000000000000000LL, 1>;
	ALIAS(Exa)		Ratio<1000000000000000000LL, 1>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class T = float_t
	> NODISCARD constexpr auto ratio_cast(T v = T{ 1 }) noexcept
	{
		if constexpr (((T)R::num == (T)1) && ((T)R::den == (T)1))
		{
			return v;
		}
		else if constexpr ((T)R::num != (T)1 && (T)R::den == (T)1)
		{
			return v * (T)R::num;
		}
		else if constexpr ((T)R::num == (T)1 && (T)R::den != (T)1)
		{
			return v / (T)R::den;
		}
		else
		{
			return v * (T)R::num / (T)R::den;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, int64_t Num, int64_t Den
	> NODISCARD constexpr auto ratio_cast(T v, Ratio<Num, Den> const & r)
	{
		auto const
			one{ static_cast<T>(1) },
			num{ static_cast<T>(r.num) },
			den{ static_cast<T>(r.den) };
		return ((num == one) && (den == one))
			? v
			: (((num != one) && (den == one))
				? v * num
				: (((num == one) && (den != one))
					? v / den
					: v * num / den));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD constexpr auto power_of_2(T v)
	{
		// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
		if constexpr (std::is_floating_point_v<T>)
		{
			return gcem::round(gcem::pow(T{ 2 }, gcem::ceil(gcem::log(v) / gcem::log(T{ 2 }))));
		}
		else
		{
			static_assert(std::is_integral_v<T>);
			v--;
			if constexpr (sizeof(T) >= 1)
				v |= v >> 1; v |= v >> 2; v |= v >> 4;	// 8-bit
			if constexpr (sizeof(T) >= 2) v |= v >> 8;	// 16-bit
			if constexpr (sizeof(T) >= 4) v |= v >> 16; // 32-bit
			if constexpr (sizeof(T) >= 8) v |= v >> 32; // 64-bit
			v++;
			return v;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// byte literals
namespace ism::byte_literals
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// kibibyte
	NODISCARD constexpr uint64_t operator"" _KiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::kilo{}));
	}

	NODISCARD constexpr uint64_t operator"" _KiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::kilo{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mebibyte
	NODISCARD constexpr uint64_t operator"" _MiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::mega{}));
	}

	NODISCARD constexpr uint64_t operator"" _MiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::mega{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// gibibyte
	NODISCARD constexpr uint64_t operator"" _GiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::giga{}));
	}

	NODISCARD constexpr uint64_t operator"" _GiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::giga{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tebibyte
	NODISCARD constexpr uint64_t operator"" _TiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::tera{}));
	}

	NODISCARD constexpr uint64_t operator"" _TiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::tera{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pebibyte
	NODISCARD constexpr uint64_t operator"" _PiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::peta{}));
	}

	NODISCARD constexpr uint64_t operator"" _PiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::peta{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// exbibyte
	NODISCARD constexpr uint64_t operator"" _EiB(uint64_t n) noexcept
	{
		return ism::power_of_2(ism::ratio_cast(n, std::exa{}));
	}

	NODISCARD constexpr uint64_t operator"" _EiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(ism::power_of_2(ism::ratio_cast(n, std::exa{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ISM_RATIO_HPP_
