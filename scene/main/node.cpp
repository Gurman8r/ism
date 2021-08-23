#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Node, t, "node")
{
	t.tp_base = typeof<Object>();
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::~Node() { clear_children(); }

	Node::Node(TYPE const & type, SceneTree * tree, Node * parent)
		: Object	{ type }
		, m_tree	{ CHECK(tree) }
		, m_parent	{ parent }
		, m_children{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}