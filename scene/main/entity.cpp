#include <scene/main/entity.hpp>

using namespace ism;

OBJECT_EMBED(Entity, t) {}

Entity::Entity()
{
	m_entity_id = get_tree()->m_entt.create();
}

Entity::~Entity()
{
	get_tree()->m_entt.destroy(m_entity_id);
}