#include <scene/components/renderer_component.hpp>

using namespace ism;

OBJECT_EMBED(RendererComponent, t) {}

RendererComponent::RendererComponent(Entity * entity) noexcept : Component{ entity } {}