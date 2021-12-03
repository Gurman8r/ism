#include <scene/components/camera_component.hpp>

using namespace ism;

EMBED_CLASS(CameraComponent, t) {}

CameraComponent::CameraComponent(Entity * entity) noexcept : BehaviorComponent{ entity } {}