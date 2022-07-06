#ifndef _ISM_CLOCK_HPP_
#define _ISM_CLOCK_HPP_

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

	// clock
	struct Clock final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using TimePoint = std::chrono::high_resolution_clock::time_point;

		static TimePoint now() noexcept { return std::chrono::high_resolution_clock::now(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Clock() noexcept = default;

		explicit Clock(bool running) noexcept
			: m_running		{ running }
			, m_start_time	{ now() }
			, m_stop_time	{ m_start_time }
			, m_elapsed		{}
		{
		}

		NON_COPYABLE(Clock);

		NON_MOVABLE(Clock);

		bool running() const noexcept
		{
			return m_running;
		}

		Duration get_elapsed_time() const noexcept
		{
			return m_running ? (now() - m_start_time) : m_elapsed;
		}

		Clock & restart() noexcept
		{
			m_running = true;

			m_start_time = m_stop_time = now();

			m_elapsed = {};

			return (*this);
		}

		Clock & stop() noexcept
		{
			if (m_running)
			{
				m_running = false;

				m_stop_time = now();

				m_elapsed = (m_stop_time - m_start_time);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_running;
		TimePoint	m_start_time;
		TimePoint	m_stop_time;
		Duration	m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLOCK_HPP_
