#ifndef _ISM_PERFORMANCE_HPP_
#define _ISM_PERFORMANCE_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FPS_Tracker final
	{
		f32			value{};
		f32			accum{};
		size_t		index{};
		Vector<f32>	times{};

		FPS_Tracker(size_t count = 120) noexcept { times.resize(count); }

		void update(Duration const & dt) noexcept
		{
			accum += (f32)dt - times[index];
			times[index] = dt;
			index = (index + 1) % times.size();
			value = (0.f < accum) ? (1.f / (accum / (f32)times.size())) : FLT_MAX;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Performance : public Object
	{
		OBJECT_CLASS(Performance, Object);

		SINGLETON_CLASS(Performance);

	public:
		Performance();
		virtual ~Performance() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PERFORMANCE_HPP_
