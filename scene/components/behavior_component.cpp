#include <scene/components/behavior_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(BehaviorComponent, t) {}

	BehaviorComponent::BehaviorComponent(Entity * entity) noexcept : Component{ entity }
	{
	}

	BehaviorComponent::~BehaviorComponent() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}