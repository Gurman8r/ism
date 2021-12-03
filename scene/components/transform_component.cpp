#include <scene/components/transform_component.hpp>

using namespace ism;

EMBED_CLASS(TransformComponent, t) {}

TransformComponent::TransformComponent(Entity * entity) noexcept : Component{ entity } {}