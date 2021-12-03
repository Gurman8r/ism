#include <scene/components/script_component.hpp>

using namespace ism;

EMBED_CLASS(ScriptComponent, t) {}

ScriptComponent::ScriptComponent(Entity * entity) noexcept : BehaviorComponent{ entity }
{
}

ScriptComponent::~ScriptComponent() noexcept
{
}