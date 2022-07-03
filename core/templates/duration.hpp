#ifndef _ISM_DURATION_HPP_
#define _ISM_DURATION_HPP_

#include <core/templates/ratio.hpp>
#include <chrono>

// duration
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public std::chrono::duration<float_t>
	{
		using std::chrono::duration<float_t>::duration;

		constexpr operator float_t() const noexcept { return count(); }

		template <class T> constexpr operator T() const noexcept { return (T)count(); }

		constexpr auto nanoseconds() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Nano>>(*this);
		}

		constexpr auto microseconds() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Micro>>(*this);
		}

		constexpr auto milliseconds() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Milli>>(*this);
		}

		constexpr auto seconds() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<1>>>(*this);
		}

		constexpr auto minutes() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60>>>(*this);
		}

		constexpr auto hours() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60 * 60>>>(*this);
		}

		constexpr auto days() const noexcept -> Duration {
			return std::chrono::duration_cast<std::chrono::duration<float_t, Ratio<60 * 60 * 24>>>(*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _ns(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Nano()) };
	}

	constexpr auto operator"" _ns(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Nano()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _us(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Micro()) };
	}

	constexpr auto operator"" _us(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Micro()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _ms(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Milli()) };
	}

	constexpr auto operator"" _ms(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Milli()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _s(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<1>()) };
	}

	constexpr auto operator"" _s(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<1>()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _min(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60>()) };
	}

	constexpr auto operator"" _min(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60>()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _hrs(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60>()) };
	}

	constexpr auto operator"" _hrs(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60>()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _days(float80_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60 * 24>()) };
	}

	constexpr auto operator"" _days(uint64_t value) noexcept {
		return Duration{ ratio_cast(static_cast<float_t>(value), Ratio<60 * 60 * 24>()) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// timer
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Timer final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using time_point = std::chrono::high_resolution_clock::time_point;

		static time_point now() noexcept { return std::chrono::high_resolution_clock::now(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Timer(bool running) noexcept
			: m_running		{ running }
			, m_start_time	{ now() }
			, m_stop_time	{ m_start_time }
			, m_elapsed		{}
		{
		}

		Timer() noexcept = default;

		NON_COPYABLE(Timer);

		NON_MOVABLE(Timer);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool running() const noexcept
		{
			return m_running;
		}

		Duration elapsed() const noexcept
		{
			if (m_running) { return now() - m_start_time; }

			else { return m_elapsed; }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer & start() noexcept
		{
			if (m_running) { return *this; }

			else { return restart(); }
		}

		Timer & restart() noexcept
		{
			m_running = true;

			m_start_time = m_stop_time = now();

			m_elapsed = {};

			return (*this);
		}

		Timer & stop() noexcept
		{
			if (!m_running) { return (*this); }

			m_running = false;

			m_stop_time = now();

			m_elapsed = (m_stop_time - m_start_time);

			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_running;
		time_point	m_start_time;
		time_point	m_stop_time;
		Duration	m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DURATION_HPP_
