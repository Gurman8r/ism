#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Node, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::Node()
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

	void Node::_notification(i32 value)
	{
		switch (value)
		{
		case Notification_EnterTree: {
		} break;
		case Notification_ExitTree: {
		} break;
		case Notification_Ready: {
		} break;
		case Notification_Paused: {
		} break;
		case Notification_Unpaused: {
		} break;
		case Notification_Process: {
		} break;
		case Notification_PhysicsProcess: {
		} break;
		case Notification_Parented: {
		} break;
		case Notification_Unparented: {
		} break;
		case Notification_Instanced: {
		} break;
		case Notification_DragBegin: {
		} break;
		case Notification_DragEnd: {
		} break;
		case Notification_PathChanged: {
		} break;
		case Notification_Internal_Process: {
		} break;
		case Notification_Internal_PhysicsProcess: {
		} break;
		case Notification_MemoryWarning: {
		} break;
		case Notification_Crash: {
		} break;
		case Notification_ApplicationResumed: {
		} break;
		case Notification_ApplicationPaused: {
		} break;
		case Notification_ApplicationFocusIn: {
		} break;
		case Notification_ApplicationFocusOut: {
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Node::propagate_notification(i32 notification_id, bool reverse)
	{
		notification(notification_id, reverse);

		for (Node * child : m_nodes)
		{
			child->propagate_notification(notification_id, reverse);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Node::set_tree(SceneTree * tree)
	{
		if (!tree || m_tree == tree) { return false; }
		m_tree = tree;
		for (Node * child : m_nodes) { child->set_tree(tree); }
		return true;
	}

	bool Node::set_parent(Node * parent)
	{
		if (!parent || this == parent || m_parent == parent) { return false; }

		// add to new parent
		parent->m_nodes.push_back(this);

		// remove from old parent
		if (m_parent) {
			m_parent->m_nodes.erase(m_parent->m_nodes.begin() + get_sibling_index());
		}

		m_parent = parent;
		m_tree = parent->m_tree;
		return true;
	}

	size_t Node::get_sibling_index() const
	{
		if (m_parent)
		{
			for (size_t i{}; i < m_parent->m_nodes.size(); ++i)
			{
				if (this == m_parent->m_nodes[i])
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

		ASSERT(new_index < m_parent->m_nodes.size());
		
		size_t const old_index{ get_sibling_index() };

		if (new_index == old_index) { return; }
		
		util::swap(m_parent->m_nodes[new_index], m_parent->m_nodes[old_index]);
	}

	Node * Node::add_child(Node * child)
	{
		return (child && child->set_parent(this)) ? child : nullptr;
	}

	void Node::destroy_child(size_t i)
	{
		ASSERT(i < m_nodes.size());

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