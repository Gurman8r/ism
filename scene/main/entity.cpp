#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(Entity, t, TypeFlags_BaseType) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Entity::Entity(SceneTree * scene, Node * parent) noexcept : Node{ scene, parent }
{
	m_entity_id = m_scene->m_entt.create();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */