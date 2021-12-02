#include <scene/components/mono_behavior.hpp>

using namespace ism;

EMBEDED_CLASS(MonoBehavior, t) {}

MonoBehavior::MonoBehavior(Entity * entity) noexcept : Behavior{ entity }
{
}

MonoBehavior::~MonoBehavior() noexcept
{
}