#ifndef _ISM_MONO_BEHAVIOR_HPP_
#define _ISM_MONO_BEHAVIOR_HPP_

#include <scene/components/behavior.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MonoBehavior : public Behavior
	{
		OBJECT_COMMON(MonoBehavior, Behavior);

	protected:
		MonoBehavior(Entity * entity) noexcept;

	public:
		virtual ~MonoBehavior() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MONO_BEHAVIOR_HPP_
