#include <runtime/scene/entity.hpp>
#include <runtime/scene/scene_tree.hpp>

using namespace ism;

OBJECT_IMPL(Entity, t) {}

Entity::~Entity() {}

Entity::Entity(SceneTree * tree, Node * parent) : Node{ tree, parent }, m_entity{ get_tree()->get_registry()->create() } {}

Entity::Entity(SceneTree * tree) : Node{ tree }, m_entity{ get_tree()->get_registry()->create() } {}

Entity::Entity(Node * parent) : Node{ parent }, m_entity{ get_tree()->get_registry()->create() } {}