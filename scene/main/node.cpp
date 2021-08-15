#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::~Node() { clear_children(); }

	Node::Node(SceneTree * tree, Node * parent)
		: m_tree	{ CHECK(tree) }
		, m_parent	{ parent }
		, m_children{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}