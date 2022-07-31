#include <scene/components/transform_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(TransformComponent, t) {}

	TransformComponent::TransformComponent(Entity * entity) noexcept : Component{ entity } {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}