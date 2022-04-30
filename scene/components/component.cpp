#include <scene/components/component.hpp>

using namespace ism;

OBJECT_EMBED(Component, t) {}

Component::Component(Entity * entity) noexcept : m_entity{ VALIDATE(entity) }
{
}

Component::~Component() noexcept
{
	m_entity = nullptr;
}