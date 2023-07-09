#ifndef _ISM_CAMERA_BEHAVIOR_HPP_
#define _ISM_CAMERA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace Ism
{
	class ISM_API CameraBehavior : public Behavior
	{
		OBJECT_CLASS(CameraBehavior, Behavior);

	public:
		CameraBehavior();
		virtual ~CameraBehavior() override;
	};
}

#endif // !_ISM_CAMERA_BEHAVIOR_HPP_
