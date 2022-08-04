#include <scene/components/camera_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(CameraComponent, t) {}

	CameraComponent::CameraComponent(Entity * entity) noexcept : BehaviorComponent{ entity } {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}