#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(Node, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Node::~Node() { clear_children(); }

Node::Node(SceneTree * tree, Node * parent)
	: Object{}
	, m_tree{ CHECK(tree) }
	, m_parent{ parent }
	, m_children{}
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */