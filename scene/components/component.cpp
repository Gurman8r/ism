#include <scene/components/component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Component, t) {}

	Component::Component(Entity * entity) noexcept : m_entity{ VALIDATE(entity) }
	{
	}

	Component::~Component() noexcept
	{
		m_entity = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}