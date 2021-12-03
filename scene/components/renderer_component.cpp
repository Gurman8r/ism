#include <scene/components/renderer_component.hpp>

using namespace ism;

EMBED_CLASS(RendererComponent, t) {}

RendererComponent::RendererComponent(Entity * entity) noexcept : Component{ entity } {}