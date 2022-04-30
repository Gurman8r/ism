#include <scene/components/behavior_component.hpp>

using namespace ism;

OBJECT_EMBED(BehaviorComponent, t) {}

BehaviorComponent::BehaviorComponent(Entity * entity) noexcept : Component{ entity }
{
}

BehaviorComponent::~BehaviorComponent() noexcept
{
}