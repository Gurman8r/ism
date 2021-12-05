#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Node, t, TypeFlags_IsAbstract)
{
	t.tp_bind = CLASS_BINDER(Node, t)
	{
		return t
			.def("process", &Node::process)
			.def("handle_event", &Node::handle_event)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Node::Node(SceneTree * tree) noexcept : m_tree{ tree ? tree : VALIDATE(SINGLETON(SceneTree)) }
{
}

Node::~Node() noexcept
{
	while (!m_nodes.empty()) { m_nodes.pop_back(); }
}

void Node::process(Duration const & dt)
{
	Vector<size_t> to_remove{};

	for (size_t i = 0, imax = get_child_count(); i < imax; ++i)
	{
		if (NODE node{ get_child(i) })
		{
			node->process(dt);
		}
		else
		{
			to_remove.push_back(i);
		}
	}

	while (!to_remove.empty())
	{
		destroy_child(to_remove.back());

		to_remove.pop_back();
	}
}

void Node::handle_event(Event const & event)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Node::detach_children()
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

bool Node::set_parent(Node * value)
{
	if (!value || (this == value) || m_owner == value) { return false; }

	value->m_nodes.push_back(this);

	if (m_owner) { m_owner->destroy_child(get_sibling_index()); }

	m_owner = value;

	m_tree = value->m_tree;

	return true;
}

size_t Node::get_sibling_index() const noexcept
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

void Node::set_sibling_index(size_t const i)
{
	if (!m_owner) { return; }

	auto & siblings{ m_owner->m_nodes };

	ASSERT(i < siblings.size());

	NODE self{ this };

	siblings.erase(siblings.begin() + get_sibling_index());

	siblings.insert(siblings.begin() + i, self);
}

bool Node::is_child_of(Node const * other, bool recursive) const noexcept
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

bool Node::is_parent_of(Node const * other, bool recursive) const noexcept
{
	if (!other || (this == other)) { return false; }
	else if (this == other->m_owner) { return true; }
	else if (recursive)
	{
		for (NODE const & node : m_nodes)
		{
			if (node->is_parent_of(other, true))
			{
				return true;
			}
		}
	}
	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */