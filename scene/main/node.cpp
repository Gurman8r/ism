#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::~Node() { clear_children(); }

	Node::Node(Node * parent, SceneTree * tree)
		: m_tree	{ CHECK(tree) }
		, m_entity	{ tree->m_reg.create() }
		, m_parent	{ parent }
		, m_children{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}