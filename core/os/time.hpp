#ifndef _ISM_TIME_HPP_
#define _ISM_TIME_HPP_

#include <core/templates/ratio.hpp>

#include <chrono>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// duration base
	ALIAS(_DurationBase) std::chrono::duration<f32>;

	// duration
	class Duration : public _DurationBase
	{
	public:
		using base_type = _DurationBase;
		using base_type::base_type;
		using base_type::count;
		using base_type::operator+;
		using base_type::operator-;
		using base_type::operator+=;
		using base_type::operator-=;
		using base_type::operator*=;
		using base_type::operator/=;

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr operator T() const noexcept { return (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr Duration & operator=(T const value) noexcept { return (*this) = Duration{ (rep)value }; }

		constexpr Duration & operator++() noexcept { return (*this) = count() + (rep)1; }
		constexpr Duration operator++(int) noexcept { Duration temp{ *this }; return ++temp, temp; }

		constexpr Duration & operator--() noexcept { return (*this) = count() - (rep)1; }
		constexpr Duration operator--(int) noexcept { Duration temp{ *this }; return --temp, temp; }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator==(T const value) const noexcept { return value == (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator!=(T const value) const noexcept { return value != (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator<(T const value) const noexcept { return value < (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator>(T const value) const noexcept { return value > (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator<=(T const value) const noexcept { return value <= (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator>=(T const value) const noexcept { return value >= (T)count(); }

		template <class T = u32> constexpr auto nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Nano>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Micro>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto milliseconds() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Milli>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto seconds() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Ratio<1>>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto minutes() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Ratio<60>>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto hours() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Ratio<60 * 60>>>((_DurationBase const &)*this).count(); }
		template <class T = u32> constexpr auto days() const noexcept { return std::chrono::duration_cast<std::chrono::duration<T, Ratio<60 * 60 * 24>>>((_DurationBase const &)*this).count(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD constexpr Duration operator+(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c += b; }

	NODISCARD constexpr Duration operator-(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c -= b; }

	NODISCARD constexpr Duration operator*(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c *= b; }

	NODISCARD constexpr Duration operator/(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator+=(T & a, Duration const & b) noexcept -> T & { return a = a + (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator-=(T & a, Duration const & b) noexcept -> T & { return a = a - (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator*=(T & a, Duration const & b) noexcept -> T & { return a = a * (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator/=(T & a, Duration const & b) noexcept -> T & { return a = a / (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator+(T const a, Duration const & b) noexcept { auto c{ a }; return c += b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator-(T const a, Duration const & b) noexcept { auto c{ a }; return c -= b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator*(T const a, Duration const & b) noexcept { auto c{ a }; return c *= b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator/(T const a, Duration const & b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator+=(Duration & a, T const b) noexcept { return a = (Duration::rep)a + (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator-=(Duration & a, T const b) noexcept { return a = (Duration::rep)a - (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator*=(Duration & a, T const b) noexcept { return a = (Duration::rep)a * (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator/=(Duration & a, T const b) noexcept { return a = (Duration::rep)a / (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator+(Duration const & a, T const b) noexcept { auto c{ a }; return c += b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator-(Duration const & a, T const b) noexcept { auto c{ a }; return c -= b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator*(Duration const & a, T const b) noexcept { auto c{ a }; return c *= b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator/(Duration const & a, T const b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _ns(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Nano{}) }; }
	constexpr auto operator"" _ns(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Nano{}) }; }

	constexpr auto operator"" _us(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Micro{}) }; }
	constexpr auto operator"" _us(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Micro{}) }; }

	constexpr auto operator"" _ms(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Milli{}) }; }
	constexpr auto operator"" _ms(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Milli{}) }; }

	constexpr auto operator"" _s(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<1>{}) }; }
	constexpr auto operator"" _s(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<1>{}) }; }

	constexpr auto operator"" _min(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60>{}) }; }
	constexpr auto operator"" _min(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60>{}) }; }

	constexpr auto operator"" _hrs(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60>{}) }; }
	constexpr auto operator"" _hrs(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60>{}) }; }

	constexpr auto operator"" _days(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60 * 24>{}) }; }
	constexpr auto operator"" _days(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60 * 24>{}) }; }

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
