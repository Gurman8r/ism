#include <scene/components/camera_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(CameraComponent, t) {}

	CameraComponent::CameraComponent(Entity * entity) noexcept : BehaviorComponent{ entity } {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}