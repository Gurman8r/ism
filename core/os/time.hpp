#ifndef _ISM_TIME_HPP_
#define _ISM_TIME_HPP_

#include <core/templates/ratio.hpp>

#include <chrono>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// duration base
	ALIAS(_DurationBase) std::chrono::duration<float_t>;

	// duration
	struct Duration : public _DurationBase
	{
		using base_type = _DurationBase;
		using base_type::base_type;
		using base_type::count;

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr operator T() const noexcept { return (T)count(); }

		constexpr auto nanoseconds() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Nano>>(*this); }
		constexpr auto microseconds() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Micro>>(*this); }
		constexpr auto milliseconds() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Milli>>(*this); }
		constexpr auto seconds() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<1>>>(*this); }
		constexpr auto minutes() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60>>>(*this); }
		constexpr auto hours() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60 * 60>>>(*this); }
		constexpr auto days() const noexcept -> Duration { return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60 * 60 * 24>>>(*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator+=(T & a, Duration const & b) noexcept -> T & { return a = a + (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator-=(T & a, Duration const & b) noexcept -> T & { return a = a - (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator*=(T & a, Duration const & b) noexcept -> T & { return a = a * (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator/=(T & a, Duration const & b) noexcept -> T & { return a = a / (T)b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator+=(Duration & a, T const b) noexcept { return a = (Duration::rep)a + (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator-=(Duration & a, T const b) noexcept { return a = (Duration::rep)a - (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator*=(Duration & a, T const b) noexcept { return a = (Duration::rep)a * (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator/=(Duration & a, T const b) noexcept { return a = (Duration::rep)a / (Duration::rep)b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> constexpr auto operator+(T const & a, Duration const & b) noexcept { auto c{ a }; return c += b; }
	template <class T> constexpr auto operator-(T const & a, Duration const & b) noexcept { auto c{ a }; return c -= b; }
	template <class T> constexpr auto operator*(T const & a, Duration const & b) noexcept { auto c{ a }; return c *= b; }
	template <class T> constexpr auto operator/(T const & a, Duration const & b) noexcept { auto c{ a }; return c /= b; }
	template <class T> constexpr auto operator+(Duration const & a, T const & b) noexcept { auto c{ a }; return c += b; }
	template <class T> constexpr auto operator-(Duration const & a, T const & b) noexcept { auto c{ a }; return c -= b; }
	template <class T> constexpr auto operator*(Duration const & a, T const & b) noexcept { auto c{ a }; return c *= b; }
	template <class T> constexpr auto operator/(Duration const & a, T const & b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _ns(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Nano{}) }; }
	constexpr auto operator"" _ns(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Nano{}) }; }

	constexpr auto operator"" _us(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Micro{}) }; }
	constexpr auto operator"" _us(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Micro{}) }; }

	constexpr auto operator"" _ms(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Milli{}) }; }
	constexpr auto operator"" _ms(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Milli{}) }; }

	constexpr auto operator"" _s(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<1>{}) }; }
	constexpr auto operator"" _s(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<1>{}) }; }

	constexpr auto operator"" _min(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60>{}) }; }
	constexpr auto operator"" _min(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60>{}) }; }

	constexpr auto operator"" _hrs(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60>{}) }; }
	constexpr auto operator"" _hrs(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60>{}) }; }

	constexpr auto operator"" _days(float80_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60 * 24>{}) }; }
	constexpr auto operator"" _days(uint64_t value) noexcept { return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60 * 24>{}) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// clock base
	ALIAS(_ClockBase) std::chrono::high_resolution_clock;

	// clock
	class Clock final
	{
		_ClockBase::time_point m_time;

	public:
		Clock() noexcept : m_time{ _ClockBase::now() } {}

		Duration get_elapsed_time() const noexcept { return _ClockBase::now() - m_time; }

		void restart() noexcept { m_time = _ClockBase::now(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TIME_HPP_
