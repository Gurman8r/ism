#include <scene/components/script_component.hpp>

using namespace ism;

EMBED_OBJECT_CLASS(ScriptComponent, t) {}

ScriptComponent::ScriptComponent(Entity * entity) noexcept : BehaviorComponent{ entity }
{
}

ScriptComponent::~ScriptComponent() noexcept
{
}