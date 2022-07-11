#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	// node
	class ISM_API Node : public Object
	{
		OBJECT_COMMON(Node, Object);

		friend class SceneTree;

	public:
		enum
		{
			Notification_EnterTree = 10,
			Notification_ExitTree,
			Notification_Ready,
			Notification_Paused,
			Notification_Unpaused,
			Notification_Process,
			Notification_PhysicsProcess,
			Notification_Parented,
			Notification_Unparented,
			Notification_Instanced,
			Notification_DragBegin,
			Notification_DragEnd,
			Notification_PathChanged,
			Notification_Internal_Process,
			Notification_Internal_PhysicsProcess,
			Notification_PostEnterTree,

			Notification_WM_MouseEnter = 1001,
			Notification_WM_MouseExit,
			Notification_WM_FocusIn,
			Notification_WM_FocusOut,
			Notification_WM_CloseRequest,
			Notification_WM_GoBackRequest,
			Notification_WM_SizeChanged,
			Notification_WM_ScaleChanged,

			Notification_MemoryWarning = MainLoop::Notification_MemoryWarning,
			Notification_Crash = MainLoop::Notification_Crash,
			Notification_ApplicationResumed	 = MainLoop::Notification_ApplicationResumed,
			Notification_ApplicationPaused	 = MainLoop::Notification_ApplicationPaused,
			Notification_ApplicationFocusIn	 = MainLoop::Notification_ApplicationFocusIn,
			Notification_ApplicationFocusOut = MainLoop::Notification_ApplicationFocusOut,
		};

	protected:
		SceneTree *		m_tree{};
		Node *			m_parent{};
		Vector<Node *>	m_nodes{};

		Node();

	public:
		virtual ~Node() override;

		void propagate_notification(int32_t notification_id, bool reverse = false);

	protected:
		void _notification(int32_t notification_id);

	public:
		SceneTree * get_tree() const noexcept { return m_tree; }
		bool set_tree(SceneTree * tree);

		Node * get_parent() const noexcept { return m_parent; }
		bool set_parent(Node * parent);

		size_t get_sibling_index() const;
		void set_sibling_index(size_t i);

		Node * get_child(size_t i) const noexcept { ASSERT(i < get_child_count()); return m_nodes[i]; }
		size_t get_child_count() const noexcept { return m_nodes.size(); }

		template <class T, class ... Args
		> T * add_child(Args && ... args) { return (T *)add_child(memnew(T(args...))); }
		Node * add_child(Node * child);
		void destroy_child(size_t i);
		
		bool is_child_of(Node const * parent, bool recursive = false) const;
		bool is_parent_of(Node const * child, bool recursive = false) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
