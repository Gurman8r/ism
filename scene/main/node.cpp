#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Node, t, TypeFlags_IsAbstract)
{
	CLASS_DEF(Node, t)
	{
		return t
			.def("_process", &Node::process)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Node::Node() noexcept : m_tree{ VALIDATE(SINGLETON(SceneTree)) } {}

Node::~Node() noexcept { while (!m_children.empty()) { m_children.pop_back(); } }

void Node::process(Duration const & dt)
{
	for (NODE & n : m_children)
	{
		n->process(dt);
	}
}

void Node::handle_event(Event const & ev)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Node::detach_nodes()
{
	if (m_parent)
	{
		for (NODE const & node : m_children)
		{
			node->m_parent = m_parent;

			m_parent->m_children.push_back(node);
		}
	}
	else
	{
		for (NODE const & node : m_children)
		{
			node->m_parent = nullptr;
		}
	}

	m_children.clear();
}

bool Node::set_owner(Node * value)
{
	if (!value || (this == value) || m_parent == value) { return false; }

	value->m_children.push_back(this);

	if (m_parent) { m_parent->delete_node(get_sibling_index()); }

	m_parent = value;

	m_tree = value->m_tree;

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t Node::get_sibling_index() const noexcept
{
	return !m_parent
		? npos
		: (size_t)std::distance(
			m_parent->m_children.begin(),
			std::find(
				m_parent->m_children.begin(),
				m_parent->m_children.end(),
				this));
}

void Node::set_sibling_index(size_t const i)
{
	if (!m_parent) { return; }

	auto & siblings{ m_parent->m_children };

	VERIFY(i < siblings.size());

	NODE self{ this };

	siblings.erase(siblings.begin() + get_sibling_index());

	siblings.insert(siblings.begin() + i, self);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Node::is_owned_by(Node const * other, bool recursive) const noexcept
{
	if (!m_parent || !other || (this == other)) { return false; }
	else if (m_parent == other) { return true; }
	else if (recursive)
	{
		Node * it{ m_parent->m_parent };
		while (it)
		{
			if (it == other) {
				return true;
			}
			it = it->m_parent;
		}
	}
	return false;
}

bool Node::is_owner_of(Node const * other, bool recursive) const noexcept
{
	if (!other || (this == other)) { return false; }
	else if (this == other->m_parent) { return true; }
	else if (recursive)
	{
		for (NODE const & node : m_children)
		{
			if (node->is_owner_of(other, true))
			{
				return true;
			}
		}
	}
	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */