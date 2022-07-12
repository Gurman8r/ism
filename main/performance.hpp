#ifndef _ISM_PERFORMANCE_HPP_
#define _ISM_PERFORMANCE_HPP_

#include <core/os/time.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	struct FPS_Tracker final
	{
		f32			value{};
		f32			accum{};
		size_t			index{};
		Vector<f32>	times{};

		FPS_Tracker(size_t count = 120) noexcept : value{}, accum{}, index{}, times{}
		{
			times.resize(count);
		}

		void update(Duration const & dt) noexcept
		{
			accum += (f32)dt - times[index];
			times[index] = dt;
			index = (index + 1) % times.size();
			value = (0.f < accum) ? (1.f / (accum / (f32)times.size())) : FLT_MAX;
		}
	};
}

#endif // !_ISM_PERFORMANCE_HPP_
