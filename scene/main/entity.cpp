#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

EMBEDDED_CLASS_TYPE(Entity, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Entity::Entity() : Node{}
{
	m_entity_id = get_tree()->m_entt.create();
}

Entity::~Entity()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */