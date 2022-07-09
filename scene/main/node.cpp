#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Node, t, TypeFlags_IsAbstract)
	{
		t.tp_install = CLASS_INSTALLER(Node, t)
		{
			return t
				.def("handle_event", &Node::handle_event)
				;
		};
	}

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

	void Node::initialize()
	{
		each_child([&](Node * n) { n->initialize(); });
	}

	void Node::finalize()
	{
		each_child([&](Node * n) { n->finalize(); });
	}

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
		each_child([&](Node * n) { n->notification(id); });
	}

	void Node::handle_event(Event const & event)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Node::set_parent(Node * parent)
	{
		if (!parent || this == parent || m_parent == parent) { return false; }

		// add self to new parent
		parent->m_nodes.push_back(this);

		// remove self from old parent
		if (m_parent) { m_parent->m_nodes.erase(m_parent->m_nodes.begin() + get_sibling_index()); }

		// set new parent
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
		return npos;
	}

	void Node::set_sibling_index(size_t new_index)
	{
		if (!m_parent) { return; }
		
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

	void Node::destroy_child(size_t index)
	{
		ASSERT(index < get_child_count());

		auto const it{ m_nodes.begin() + index };

		m_nodes.erase(it);
	}

	void Node::destroy_children()
	{
		while (!m_nodes.empty())
		{
			memdelete(m_nodes.back());

			m_nodes.pop_back();
		}
		m_nodes.clear();
	}

	bool Node::is_child_of(Node const * parent, bool recursive) const
	{
		if (!m_parent || !parent || (this == parent)) { return false; }
		else if (m_parent == parent) { return true; }
		else if (recursive)
		{
			Node * it{ m_parent->m_parent };
			while (it)
			{
				if (it == parent) {
					return true;
				}
				it = it->m_parent;
			}
		}
		return false;
	}

	bool Node::is_parent_of(Node const * child, bool recursive) const
	{
		if (!child || (this == child)) { return false; }
		else if (this == child->m_parent) { return true; }
		else if (recursive)
		{
			for (auto const & node : m_nodes)
			{
				if (node->is_parent_of(child, true))
				{
					return true;
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}