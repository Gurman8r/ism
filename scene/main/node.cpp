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
			.def("process", &Node::process)
			.def("handle_event", &Node::handle_event)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Node::Node() noexcept : m_tree{ VALIDATE(SINGLETON(SceneTree)) }
{
}

Node::~Node() noexcept
{
	while (!m_nodes.empty()) { m_nodes.pop_back(); }
}

void Node::process(Duration const & dt)
{
	for (size_t i = 0, imax = get_node_count(); i < imax; ++i)
	{
		get_node(i)->process(dt);
	}
}

void Node::handle_event(Event const & event)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	if (m_owner) { m_owner->delete_node(get_subindex()); }

	m_owner = value;

	m_tree = value->m_tree;

	return true;
}

size_t Node::get_subindex() const noexcept
{
	return !m_owner
		? npos
		: (size_t)std::distance(
			m_owner->m_nodes.begin(),
			std::find(
				m_owner->m_nodes.begin(),
				m_owner->m_nodes.end(),
				this));
}

void Node::set_subindex(size_t const i)
{
	if (!m_owner) { return; }

	auto & siblings{ m_owner->m_nodes };

	VERIFY(i < siblings.size());

	NODE self{ this };

	siblings.erase(siblings.begin() + get_subindex());

	siblings.insert(siblings.begin() + i, self);
}

bool Node::is_owned_by(Node const * other, bool recursive) const noexcept
{
	if (!m_owner || !other || (this == other)) { return false; }
	else if (m_owner == other) { return true; }
	else if (recursive)
	{
		Node * it{ m_owner->m_owner };
		while (it)
		{
			if (it == other) {
				return true;
			}
			it = it->m_owner;
		}
	}
	return false;
}

bool Node::is_owner_of(Node const * other, bool recursive) const noexcept
{
	if (!other || (this == other)) { return false; }
	else if (this == other->m_owner) { return true; }
	else if (recursive)
	{
		for (NODE const & node : m_nodes)
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