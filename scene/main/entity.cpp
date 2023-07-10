#include <scene/main/entity.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Entity, t) {}

	Entity::Entity()
	{
		m_entity_id = VALIDATE(m_data.tree)->m_ecs.create();
	}

	Entity::~Entity()
	{
		VALIDATE(m_data.tree)->m_ecs.destroy(m_entity_id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}