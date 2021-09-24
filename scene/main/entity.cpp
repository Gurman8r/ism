#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

OBJECT_IMP(Entity, t)
{
}

Entity::~Entity() {}

Entity::Entity(SceneTree * tree, Node * parent) : Node{ tree, parent }, m_entity{ get_tree()->get_reg()->create() } {}

Entity::Entity(SceneTree * tree) : Node{ tree }, m_entity{ get_tree()->get_reg()->create() } {}

Entity::Entity(Node * parent) : Node{ parent }, m_entity{ get_tree()->get_reg()->create() } {}