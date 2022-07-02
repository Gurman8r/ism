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
		SceneTree *		m_tree	{};
		Node *			m_owner	{};
		Vector<Ref<Node>>	m_nodes	{};

		Node(SceneTree * tree = nullptr) noexcept;

	public:
		static constexpr size_t npos{ static_cast<size_t>(-1) };

		virtual ~Node() noexcept override;

		virtual void process(Duration const & dt);

		virtual void handle_event(Event const & event) override;

	public:
		NODISCARD auto get_child(size_t const i) const -> Ref<Node> { return m_nodes[i]; }

		NODISCARD auto get_child_count() const noexcept -> size_t { return m_nodes.size(); }

		NODISCARD auto get_parent() const noexcept -> Node * { return m_owner; }

		NODISCARD auto get_tree() const noexcept -> SceneTree * { return m_tree; }

		Ref<Node> add_child(Node * value) noexcept { return (value && value->set_parent(this)) ? value : nullptr; }

		Ref<Node> add_child(Ref<Node> const & value) noexcept { return add_child(*value); }

		template <class T, class ... Args
		> Ref<Node> add_child(Args && ... args) noexcept { return add_child(memnew(T(FWD(args)...))); }

		void destroy_child(size_t const i) { m_nodes.erase(m_nodes.begin() + i); }

		void detach_children();

		bool set_parent(Node * value);

		bool set_parent(Ref<Node> const & value) noexcept { return set_parent(*value); }

		NODISCARD size_t get_sibling_index() const noexcept;

		void set_sibling_index(size_t const i);

		NODISCARD bool is_child_of(Node const * other, bool recursive = false) const noexcept;

		NODISCARD bool is_child_of(Ref<Node> const & other, bool recursive = false) const noexcept { return is_child_of(*other, recursive); }

		NODISCARD bool is_parent_of(Node const * other, bool recursive = false) const noexcept;

		NODISCARD bool is_parent_of(Ref<Node> const & other, bool recursive = false) const noexcept { return is_parent_of(*other, recursive); }

	public:
		template <class Fn = void(*)(Ref<Node> &)
		> void for_nodes(Fn && fn, bool recursive = true, bool reverse = false)
		{
			auto _for_nodes = [](auto first, auto last, auto fn, bool recursive, bool reverse)
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
				_for_nodes(m_nodes.begin(), m_nodes.end(), FWD(fn), recursive, reverse);
			}
			else
			{
				_for_nodes(m_nodes.rbegin(), m_nodes.rend(), FWD(fn), recursive, reverse);
			}
		}

	public:
		template <class Pr = bool(*)(Ref<Node> const &)
		> NODISCARD Ref<Node> find_if(Pr && pr, bool recursive = true, bool reverse = false) const noexcept
		{
			auto _find_node_if = [](auto first, auto last, auto pr, bool recursive, bool reverse) noexcept -> Ref<Node>
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
				return _find_node_if(m_nodes.begin(), m_nodes.end(), FWD(pr), recursive, reverse);
			}
			else
			{
				return _find_node_if(m_nodes.rbegin(), m_nodes.rend(), FWD(pr), recursive, reverse);
			}
		}

		template <class T
		> NODISCARD Ref<T> find(bool recursive = true, bool reverse = false) const noexcept
		{
			return find_if([](Ref<Node> const & node) noexcept { return Ref<T>(node).is_valid(); }, recursive, reverse);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
