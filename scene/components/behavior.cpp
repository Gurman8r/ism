#include <scene/components/behavior.hpp>

using namespace ism;

EMBEDED_CLASS(Behavior, t) {}

Behavior::Behavior(Entity * entity) noexcept : Component{ entity }
{
}

Behavior::~Behavior() noexcept
{
}