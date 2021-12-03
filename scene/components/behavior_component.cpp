#include <scene/components/behavior_component.hpp>

using namespace ism;

EMBED_CLASS(BehaviorComponent, t) {}

BehaviorComponent::BehaviorComponent(Entity * entity) noexcept : Component{ entity }
{
}

BehaviorComponent::~BehaviorComponent() noexcept
{
}