#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(Entity, t, TypeFlags_BaseType) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Entity::Entity() : Node{}
{
	m_entity_id = m_tree->m_entt.create();
}

Entity::~Entity()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */