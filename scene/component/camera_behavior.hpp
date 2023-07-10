#ifndef _ISM_CAMERA_BEHAVIOR_HPP_
#define _ISM_CAMERA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>
#include <servers/rendering/camera.hpp>

namespace Ism
{
	class ISM_API CameraBehavior : public Behavior
	{
		OBJECT_CLASS(CameraBehavior, Behavior);

		Camera m_camera{};

	public:
		CameraBehavior();
		virtual ~CameraBehavior() override;
	};
}

#endif // !_ISM_CAMERA_BEHAVIOR_HPP_
