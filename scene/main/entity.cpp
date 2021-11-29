#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Entity, t) {}

Entity::Entity()
{
	m_entity_id = get_tree()->get_registry()->create();
}

Entity::~Entity()
{
	get_tree()->get_registry()->destroy(m_entity_id);
}

void Entity::process(Duration const & dt)
{
	Node::process(dt);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */