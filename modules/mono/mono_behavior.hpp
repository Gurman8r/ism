#ifndef _ISM_MONO_BEHAVIOR_HPP_
#define _ISM_MONO_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace ism
{
	// mono behavior
	class ISM_MOD_API MonoBehavior : public Behavior
	{
		DEFINE_CLASS(MonoBehavior, Behavior);

	public:
		MonoBehavior();
		virtual ~MonoBehavior() override;
	};
}

#endif // !_ISM_MONO_BEHAVIOR_HPP_
