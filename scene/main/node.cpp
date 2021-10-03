#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

EMBEDDED_CLASS_TYPE(Node, t, TypeFlags_IsAbstract) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Node::~Node()
{
	while (!m_nodes.empty())
	{
		m_nodes.pop_back();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Node::delete_node(size_t const i)
{
	VERIFY(i < get_node_count());

	m_nodes.erase(m_nodes.begin() + i);
}

void Node::detach_nodes()
{
	if (m_owner)
	{
		for (NODE const & node : m_nodes)
		{
			node->m_owner = m_owner;

			m_owner->m_nodes.push_back(node);
		}
	}
	else
	{
		for (NODE const & node : m_nodes)
		{
			node->m_owner = nullptr;
		}
	}

	m_nodes.clear();
}

bool Node::set_owner(Node * value)
{
	if (!value || (this == value) || m_owner == value) { return false; }

	value->m_nodes.push_back(this);

	if (m_owner) { m_owner->delete_node(get_sibling_index()); }

	m_owner = value;

	return true;
}

void Node::set_sibling_index(size_t const i)
{
	if (!m_owner) { return; }

	auto & siblings{ m_owner->m_nodes };

	VERIFY(i < siblings.size());

	NODE self{ this };

	siblings.erase(siblings.begin() + get_sibling_index());

	siblings.insert(siblings.begin() + i, self);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */