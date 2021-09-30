#ifndef _ISM_PERFORMANCE_HPP_
#define _ISM_PERFORMANCE_HPP_

#include <core/templates/duration.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	struct FPS_Tracker final
	{
		float_t			value{};
		float_t			accum{};
		size_t			index{};
		Vector<float_t>	times{};

		FPS_Tracker(size_t count = 0, PolymorphicAllocator<byte> alloc = {}) noexcept
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
