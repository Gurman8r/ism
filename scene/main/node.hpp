#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <scene/main/scene_tree.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum : Notification_
	{
		Notification_EnterTree = 10,
		Notification_ExitTree,
		Notification_Paused,
		Notification_Unpaused,
		Notification_Ready,
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	// node
	class ISM_API Node : public Object
	{
		OBJECT_CLASS(Node, Object);

		friend class SceneTree;

		static i32 __orphan_node_count;

	public:
		enum ProcessMode_
		{
			ProcessMode_Inherit,
			ProcessMode_Pausable,
			ProcessMode_WhenPlayed,
			ProcessMode_Always,
			ProcessMode_Disabled,
		};

	protected:
		struct Data
		{
			String			name{};
			SceneTree *		tree{};
			Node *			parent{};
			Vector<Node *>	children{};
			Viewport *		viewport{};
		}
		m_data{};

		void _notification(Notification_ id);

		virtual void _process(Duration const & dt);
		virtual void _physics_process(Duration const & dt);
		virtual void _enter_tree();
		virtual void _exit_tree();
		virtual void _ready();
		virtual void _input(InputEvent const & input_event);

	public:
		NODISCARD String get_name() const;
		void set_name(String const & name);

		NODISCARD SceneTree * get_tree() const;
		bool set_tree(SceneTree * tree);

		NODISCARD Node * get_parent() const;
		bool set_parent(Node * parent);

		NODISCARD Node * get_sibling(size_t const index) const;
		NODISCARD size_t get_sibling_index() const;
		void set_sibling_index(size_t const index);

		NODISCARD Node * get_child(size_t const index) const;

		template <class T, std::enable_if_t<std::is_base_of_v<Node, T>, int> = 0
		> NODISCARD T * get_child(size_t const index) const { return dynamic_cast<T *>(get_child(index)); }
		
		NODISCARD size_t get_child_count() const;

		Node * add_child(Node * child);

		template <class T, class ... Args, std::enable_if_t<std::is_base_of_v<Node, T>, int> = 0
		> T * add_child(Args && ... args) noexcept { return (T *)add_child((Node *)memnew(T(args...))); }

		void destroy_child(size_t const index);
		
		NODISCARD bool is_child_of(Node const * parent, bool recursive = false) const;
		NODISCARD bool is_parent_of(Node const * child, bool recursive = false) const;

		NODISCARD Window * get_root() const;

		NODISCARD Viewport * get_viewport() const { return m_data.viewport; }

		void propagate_notification(i32 id, bool reverse = false);

		Node();
		virtual ~Node() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
