#ifndef _ISM_PERFORMANCE_HPP_
#define _ISM_PERFORMANCE_HPP_

#include <core/os/clock.hpp>
#include <core/templates/list.hpp>

namespace ism
{
	struct FrameRateTracker final
	{
		float_t			value{};
		float_t			accum{};
		size_t			index{};
		List<float_t>	times{};

		operator float_t() const noexcept { return value; }

		FrameRateTracker(size_t count = 120) noexcept
			: value{}, accum{}, index{}, times{ count, 0.f, decltype(times)::allocator_type{} }
		{
		}

		void update(Duration const & dt) noexcept
		{
			accum += (float_t)dt - times[index];
			times[index] = dt;
			index = (index + 1) % times.size();
			value = (0.f < accum) ? (1.f / (accum / (float_t)times.size())) : FLT_MAX;
		}
	};
}

#endif // !_ISM_PERFORMANCE_HPP_
