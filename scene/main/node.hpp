#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/input/input.hpp>
#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class Node;

	ALIAS(EntityID) entt::entity;

	ALIAS(EntityRegistry) entt::registry;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Node : public EventHandler
	{
		OBJECT_COMMON(Node, EventHandler);

		friend class SceneTree;

	protected:
		SceneTree *			m_tree{};
		Node *				m_owner{};
		List<Ref<Node>>	m_nodes{};

		Node(SceneTree * tree = nullptr) noexcept;

	public:
		static constexpr size_t npos{ static_cast<size_t>(-1) };

		virtual ~Node() noexcept override;

		virtual void process(Duration const & dt);

		virtual void handle_event(Event const & event) override;

	public:
		SceneTree * get_tree() const noexcept { return m_tree; }

		Node * get_parent() const noexcept { return m_owner; }

		bool set_parent(Node * value);

		bool set_parent(Ref<Node> const & value);

		size_t get_sibling_index() const;

		void set_sibling_index(size_t index);

		bool is_child_of(Node const * parent, bool recursive = false) const;

		bool is_child_of(Ref<Node> const & parent, bool recursive = false) const;

		bool is_parent_of(Node const * child, bool recursive = false) const;

		bool is_parent_of(Ref<Node> const & child, bool recursive = false) const;

		Ref<Node> get_child(size_t index) const noexcept { return m_nodes[index]; }

		size_t get_child_count() const noexcept { return m_nodes.size(); }

		Ref<Node> add_child(Node * value);

		Ref<Node> add_child(Ref<Node> const & value);

		template <class T, class ... Args
		> Ref<Node> add_child(Args && ... args) noexcept { return add_child(memnew(T(FWD(args)...))); }

		void destroy_child(size_t index);

		void destroy_children();

	public:
		template <class Fn = void(*)(Ref<Node> &)
		> void each_child(Fn && fn, bool recursive = true, bool reverse = false)
		{
			auto _each_child = [](auto first, auto last, auto fn, bool recursive, bool reverse)
			{
				for (; first != last; ++first)
				{
					fn(*first);

					if (recursive)
					{
						(*first)->for_nodes(fn, true, reverse);
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

		template <class Pr = bool(*)(Ref<Node> const &)
		> Ref<Node> find_child_if(Pr && pr, bool recursive = true, bool reverse = false) const noexcept
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
						if (Ref<Node> found{ (*first)->find_node_if(pr, true, reverse) })
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

		template <class T
		> Ref<T> find(bool recursive = true, bool reverse = false) const noexcept
		{
			return find_child_if([](Ref<Node> const & node) noexcept { return Ref<T>(node).is_valid(); }, recursive, reverse);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
