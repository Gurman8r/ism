#ifndef _ISM_DURATION_HPP_
#define _ISM_DURATION_HPP_

#include <core/templates/ratio.hpp>
#include <chrono>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(Nanoseconds)	std::chrono::duration<float_t, Nano>;
	ALIAS(Microseconds)	std::chrono::duration<float_t, Micro>;
	ALIAS(Milliseconds)	std::chrono::duration<float_t, Milli>;
	ALIAS(Seconds)		std::chrono::duration<float_t, Ratio<1>>;
	ALIAS(Minutes)		std::chrono::duration<float_t, Ratio<60>>;
	ALIAS(Hours)		std::chrono::duration<float_t, Ratio<60 * 60>>;
	ALIAS(Days)			std::chrono::duration<float_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public std::chrono::duration<float_t>
	{
		DEFAULT_COPYABLE_MOVABLE(Duration);

		using std::chrono::duration<float_t>::duration;

		NODISCARD constexpr operator float_t() const noexcept { return count(); }

		template <class T> NODISCARD constexpr operator T() const noexcept { return (T)count(); }

		NODISCARD constexpr auto nanoseconds() const noexcept { return std::chrono::duration_cast<Nanoseconds>(*this); }

		NODISCARD constexpr auto microseconds() const noexcept { return std::chrono::duration_cast<Microseconds>(*this); }

		NODISCARD constexpr auto milliseconds() const noexcept { return std::chrono::duration_cast<Milliseconds>(*this); }

		NODISCARD constexpr auto seconds() const noexcept { return std::chrono::duration_cast<Seconds>(*this); }

		NODISCARD constexpr auto minutes() const noexcept { return std::chrono::duration_cast<Minutes>(*this); }

		NODISCARD constexpr auto hours() const noexcept { return std::chrono::duration_cast<Hours>(*this); }

		NODISCARD constexpr auto days() const noexcept { return std::chrono::duration_cast<Days>(*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(TimePoint) std::chrono::high_resolution_clock::time_point;

	struct Timer final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Timer(bool running) noexcept
			: m_running{ running }
			, m_start_time{ std::chrono::high_resolution_clock::now() }
			, m_stop_time{ m_start_time }
			, m_elapsed{}
		{
		}

		Timer() noexcept = default;

		Timer(Timer && other) noexcept : Timer{} { swap(std::move(other)); }

		Timer & operator=(Timer && other) noexcept { return swap(std::move(other)); }

		Timer & swap(Timer & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_running, other.m_running);
				std::swap(m_start_time, other.m_start_time);
				std::swap(m_stop_time, other.m_stop_time);
				std::swap(m_elapsed, other.m_elapsed);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD bool running() const noexcept
		{
			return m_running;
		}

		NODISCARD Duration elapsed() const noexcept
		{
			if (m_running) { return std::chrono::high_resolution_clock::now() - m_start_time; }

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

			m_start_time = m_stop_time = std::chrono::high_resolution_clock::now();

			m_elapsed = {};

			return (*this);
		}

		Timer & stop() noexcept
		{
			if (!m_running) { return (*this); }

			m_running = false;

			m_stop_time = std::chrono::high_resolution_clock::now();

			m_elapsed = (m_stop_time - m_start_time);

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

#endif // !_ISM_DURATION_HPP_
