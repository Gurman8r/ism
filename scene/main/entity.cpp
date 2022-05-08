#include <scene/main/entity.hpp>

using namespace ism;

OBJECT_EMBED(Entity, t) {}

Entity::Entity()
{
	m_entity_id = VALIDATE(m_tree)->m_entt.create();
}

Entity::~Entity()
{
	VALIDATE(m_tree)->m_entt.destroy(m_entity_id);
}