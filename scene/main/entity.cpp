#include <scene/main/entity.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Entity, t) {}

	Entity::Entity()
	{
		m_entity_id = VALIDATE(m_tree)->m_ecs.create();
	}

	Entity::~Entity()
	{
		VALIDATE(m_tree)->m_ecs.destroy(m_entity_id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}