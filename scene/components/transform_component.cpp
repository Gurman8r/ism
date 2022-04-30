#include <scene/components/transform_component.hpp>

using namespace ism;

OBJECT_EMBED(TransformComponent, t) {}

TransformComponent::TransformComponent(Entity * entity) noexcept : Component{ entity } {}