#ifndef _ISM_CAMERA_COMPONENT_HPP_
#define _ISM_CAMERA_COMPONENT_HPP_

#include <scene/components/behavior_component.hpp>

namespace ism
{
	class ISM_API CameraComponent : public BehaviorComponent
	{
		OBJECT_COMMON(CameraComponent, BehaviorComponent);

	public:
		CameraComponent(Entity * entity) noexcept;
	};
}

#endif // !_ISM_CAMERA_COMPONENT_HPP_
