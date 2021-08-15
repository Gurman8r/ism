#ifndef _ISM_TIMER_HPP_
#define _ISM_TIMER_HPP_

#include <core/templates/duration.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(HighResolutionClock) std::chrono::high_resolution_clock;

	ALIAS(TimePoint) HighResolutionClock::time_point;

	struct Timer final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Timer(bool running) noexcept
			: m_running		{ running }
			, m_start_time	{ HighResolutionClock::now() }
			, m_stop_time	{ m_start_time }
			, m_elapsed		{}
		{
		}

		Timer() noexcept = default;

		Timer(Timer && other) noexcept : Timer{} { this->swap(std::move(other)); }

		Timer & operator=(Timer && other) noexcept { this->swap(std::move(other)); return (*this); }

		void swap(Timer & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_running, other.m_running);
				std::swap(m_start_time, other.m_start_time);
				std::swap(m_stop_time, other.m_stop_time);
				std::swap(m_elapsed, other.m_elapsed);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD bool running() const noexcept
		{
			return m_running;
		}

		NODISCARD Duration elapsed() const noexcept
		{
			if (m_running)
			{
				return HighResolutionClock::now() - m_start_time;
			}
			else
			{
				return m_elapsed;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer & start() noexcept
		{
			if (m_running) { return *this; }
			else { return this->restart(); }
		}

		Timer & restart() noexcept
		{
			m_running = true;

			m_start_time = m_stop_time = HighResolutionClock::now();

			m_elapsed = {};

			return (*this);
		}

		Timer & stop() noexcept
		{
			if (!m_running) { return (*this); }

			m_running = false;

			m_stop_time = HighResolutionClock::now();

			m_elapsed = (m_stop_time - m_start_time);

			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_running	; // 
		TimePoint	m_start_time; // 
		TimePoint	m_stop_time	; // 
		Duration	m_elapsed	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_TIMER_HPP_