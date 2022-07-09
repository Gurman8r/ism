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
		enum Notification_
		{
			Notification_EnterTree = 3001,
			Notification_ExitTree,
			Notification_Ready,
			Notification_Paused,
			Notification_Unpaused,
			Notification_Update,
			Notification_FixedUpdate,
			Notification_Parented,
			Notification_Unparented,
			Notification_Instanced,
			Notification_DragBegin,
			Notification_DragEnd,
			Notification_PathChanged,
			Notification_InternalUpdate,
			Notification_InternalFixedUpdate,

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
		List<Node *>	m_nodes{};

		explicit Node(SceneTree * tree = nullptr);

	public:
		virtual ~Node() override;

		virtual void process(Duration const & dt);

		virtual void notification(int32_t id);

	public:
		SceneTree * get_tree() const noexcept { return m_tree; }

		Node * get_parent() const noexcept { return m_parent; }

		bool set_parent(Node * parent);

		size_t get_sibling_index() const;

		void set_sibling_index(size_t i);

		Node * get_child(size_t i) const noexcept { ASSERT(i < get_child_count()); return m_nodes[i]; }

		size_t get_child_count() const noexcept { return m_nodes.size(); }

		Node * add_child(Node * child);

		template <class T, class ... Args
		> T * add_child(Args && ... args) noexcept { return (T *)add_child(memnew(T(FWD(args)...))); }

		void destroy_child(size_t i);

		bool is_child_of(Node const * parent, bool recursive = false) const;

		bool is_parent_of(Node const * child, bool recursive = false) const;

	public:
		template <class Fn = void(*)(Node *)
		> void each_child(Fn && fn, bool recursive = true, bool reverse = false)
		{
			auto _each_child = [](auto first, auto last, auto fn, bool recursive, bool reverse)
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first);

					if (recursive)
					{
						(*first)->each_child(fn, true, reverse);
					}
				}
			};

			if (!reverse)
			{
				_each_child(m_nodes.begin(), m_nodes.end(), FWD(fn), recursive, reverse);
			}
			else
			{
				_each_child(m_nodes.rbegin(), m_nodes.rend(), FWD(fn), recursive, reverse);
			}
		}

		template <class Pr = bool(*)(Node *)
		> Node * find_child_if(Pr && pr, bool recursive = true, bool reverse = false) const noexcept
		{
			auto _find_child_if = [](auto first, auto last, auto pr, bool recursive, bool reverse) noexcept -> Ref<Node>
			{
				if (auto const it{ std::find_if(first, last, pr) }; it != last)
				{
					return (*it);
				}
				else if (recursive)
				{
					for (; first != last; ++first)
					{
						if (auto const found{ (*first)->find_node_if(pr, true, reverse) })
						{
							return found;
						}
					}
				}
				return nullptr;
			};

			if (!reverse)
			{
				return _find_child_if(m_nodes.begin(), m_nodes.end(), FWD(pr), recursive, reverse);
			}
			else
			{
				return _find_child_if(m_nodes.rbegin(), m_nodes.rend(), FWD(pr), recursive, reverse);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
