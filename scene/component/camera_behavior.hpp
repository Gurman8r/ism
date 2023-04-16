#ifndef _ISM_CAMERA_BEHAVIOR_HPP_
#define _ISM_CAMERA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace ism
{
	class ISM_API CameraBehavior : public Behavior
	{
		DEFINE_CLASS(CameraBehavior, Behavior);

	public:
		CameraBehavior();
		virtual ~CameraBehavior() override;
	};
}

#endif // !_ISM_CAMERA_BEHAVIOR_HPP_
