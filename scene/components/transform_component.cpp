#include <scene/components/transform_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(TransformComponent, t) {}

	TransformComponent::TransformComponent(Entity * entity) noexcept : Component{ entity } {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}