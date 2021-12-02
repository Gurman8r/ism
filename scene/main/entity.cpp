#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Entity, t) {}

Entity::Entity()
{
	m_entity_id = get_tree()->get_entt().create();
}

Entity::~Entity()
{
	get_tree()->get_entt().destroy(m_entity_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */