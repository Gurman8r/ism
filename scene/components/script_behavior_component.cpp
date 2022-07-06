#include <scene/components/script_behavior_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ScriptBehaviorComponent, t) {}

	ScriptBehaviorComponent::ScriptBehaviorComponent(Entity * entity) noexcept : BehaviorComponent{ entity }
	{
	}

	ScriptBehaviorComponent::~ScriptBehaviorComponent() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}