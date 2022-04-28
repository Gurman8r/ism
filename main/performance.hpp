#ifndef _ISM_PERFORMANCE_HPP_
#define _ISM_PERFORMANCE_HPP_

#include <core/templates/duration.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	struct NODISCARD FrameRateTracker final
	{
		float_t			value{};
		float_t			accum{};
		size_t			index{};
		Vector<float_t>	times{};

		NODISCARD operator float_t() const noexcept { return value; }

		FrameRateTracker(size_t count = 120, PolymorphicAllocator<byte> alloc = {}) noexcept
			: value{}, accum{}, index{}, times{ count, 0.f, alloc }
		{
		}

		void update(Duration const & dt) noexcept
		{
			accum += dt - times[index];
			times[index] = dt;
			index = (index + 1) % times.size();
			value = (0.f < accum) ? (1.f / (accum / (float_t)times.size())) : FLT_MAX;
		}
	};
}

#endif // !_ISM_PERFORMANCE_HPP_
