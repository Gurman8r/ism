#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Node, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::Node(SceneTree * tree) : m_tree{ tree ? tree : VALIDATE(SceneTree::get_singleton()) }
	{
	}

	Node::~Node()
	{
		while (!m_nodes.empty())
		{
			memdelete(m_nodes.back());

			m_nodes.pop_back();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Node::process(Duration const & dt)
	{
		List<size_t> to_remove{};

		for (size_t i = 0, imax = get_child_count(); i < imax; ++i)
		{
			if (Node * node{ get_child(i) })
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

	void Node::notification(int32_t id)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Node::set_parent(Node * parent)
	{
		if (!parent || this == parent || m_parent == parent) { return false; }

		// add to new parent
		parent->m_nodes.push_back(this);

		// remove from old parent
		if (m_parent) {
			m_parent->m_nodes.erase(m_parent->m_nodes.begin() + get_sibling_index());
		}

		// set parent
		m_parent = parent;
		m_tree = parent->m_tree;
		return true;
	}

	size_t Node::get_sibling_index() const
	{
		if (m_parent)
		{
			for (size_t i = 0; i < m_parent->get_child_count(); ++i)
			{
				if (this == m_parent->get_child(i))
				{
					return i;
				}
			}
		}
		return 0;
	}

	void Node::set_sibling_index(size_t new_index)
	{
		ASSERT(m_parent);

		ASSERT(new_index < m_parent->get_child_count());
		
		size_t const old_index{ get_sibling_index() };
		
		if (new_index == old_index) { return; }
		
		std::swap(m_parent->m_nodes[new_index], m_parent->m_nodes[old_index]);
	}

	Node * Node::add_child(Node * child)
	{
		if (!child) { return nullptr; }

		if (!child->set_parent(this)) { return nullptr; }

		return child;
	}

	void Node::destroy_child(size_t i)
	{
		ASSERT(i < get_child_count());

		auto const it{ m_nodes.begin() + i };

		if (*it) { memdelete(*it); }

		m_nodes.erase(it);
	}

	bool Node::is_child_of(Node const * parent, bool recursive) const
	{
		if (!parent || (this == parent) || !m_parent) { return false; }
		else if (m_parent == parent) { return true; }
		else if (recursive)
		{
			Node * it{ m_parent->m_parent };
			while (it)
			{
				if (it == parent) { return true; }
				it = it->m_parent;
			}
		}
		return false;
	}

	bool Node::is_parent_of(Node const * child, bool recursive) const
	{
		if (!child || (this == child) || !child->m_parent) { return false; }
		else if (this == child->m_parent) { return true; }
		else if (recursive)
		{
			Node * it{ child->m_parent->m_parent };
			while (it)
			{
				if (it == this) { return true; }
				it = it->m_parent;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}