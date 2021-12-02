#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/components/mono_behavior.hpp>

using namespace ism;

EMBEDED_CLASS(Entity, t) {}

Entity::Entity()
{
	m_entity_id = VALIDATE(m_tree)->m_entt.create();
}

Entity::~Entity()
{
	VALIDATE(m_tree)->m_entt.destroy(m_entity_id);
}