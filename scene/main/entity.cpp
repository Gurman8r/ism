#include <scene/main/entity.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

EMBED_CLASS(Entity, t) {}

Entity::~Entity() {}

Entity::Entity(SceneTree * tree, Node * parent) : Node{ tree, parent }, m_entity{ get_tree()->get_registry()->create() } {}

Entity::Entity(SceneTree * tree) : Node{ tree }, m_entity{ get_tree()->get_registry()->create() } {}

Entity::Entity(Node * parent) : Node{ parent }, m_entity{ get_tree()->get_registry()->create() } {}