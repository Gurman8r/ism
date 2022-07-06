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
				.def("process", &Node::process)
				.def("handle_event", &Node::handle_event)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::Node(SceneTree * tree) noexcept : m_tree{ tree ? tree : VALIDATE(SceneTree::get_singleton()) }
	{
	}

	Node::~Node() noexcept
	{
		while (!m_nodes.empty()) { m_nodes.pop_back(); }
	}

	void Node::process(Duration const & dt)
	{
		List<size_t> to_remove{};

		for (size_t i = 0, imax = get_child_count(); i < imax; ++i)
		{
			if (Ref<Node> node{ get_child(i) })
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

	bool Node::set_parent(Node * value)
	{
		if (!value || (this == value) || m_parent == value) { return false; }

		value->m_nodes.push_back(this);

		if (m_parent) { m_parent->destroy_child(get_sibling_index()); }

		m_parent = value;

		m_tree = value->m_tree;

		return true;
	}

	size_t Node::get_sibling_index() const
	{
		return !m_parent
			? npos
			: (size_t)std::distance(
				m_parent->m_nodes.begin(),
				std::find(
					m_parent->m_nodes.begin(),
					m_parent->m_nodes.end(),
					this));
	}

	void Node::set_sibling_index(size_t index)
	{
		if (!m_parent) { return; }

		auto & siblings{ m_parent->m_nodes };

		ASSERT(index < siblings.size());

		Ref<Node> self{ this };

		siblings.erase(siblings.begin() + get_sibling_index());

		siblings.insert(siblings.begin() + index, self);
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

	Ref<Node> Node::add_child(Ref<Node> value)
	{
		return (value && value->set_parent(this)) ? value : nullptr;
	}

	void Node::destroy_child(size_t index)
	{
		m_nodes.erase(m_nodes.begin() + index);
	}

	void Node::destroy_children()
	{
		if (m_parent)
		{
			for (Ref<Node> const & node : m_nodes)
			{
				node->m_parent = m_parent;

				m_parent->m_nodes.push_back(node);
			}
		}
		else
		{
			for (Ref<Node> const & node : m_nodes)
			{
				node->m_parent = nullptr;
			}
		}

		m_nodes.clear();
	}

	bool Node::is_parent_of(Node const * child, bool recursive) const
	{
		if (!child || (this == child)) { return false; }
		else if (this == child->m_parent) { return true; }
		else if (recursive)
		{
			for (Ref<Node> const & node : m_nodes)
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