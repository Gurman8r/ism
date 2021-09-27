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
}

#endif // !_ISM_DURATION_HPP_
