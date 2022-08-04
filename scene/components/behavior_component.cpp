#include <scene/components/behavior_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(BehaviorComponent, t) {}

	BehaviorComponent::BehaviorComponent(Entity * entity) noexcept : Component{ entity }
	{
	}

	BehaviorComponent::~BehaviorComponent() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}