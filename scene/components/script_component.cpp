#include <scene/components/script_component.hpp>

using namespace ism;

OBJECT_EMBED(ScriptComponent, t) {}

ScriptComponent::ScriptComponent(Entity * entity) noexcept : BehaviorComponent{ entity }
{
}

ScriptComponent::~ScriptComponent() noexcept
{
}