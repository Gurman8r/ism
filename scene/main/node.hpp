#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/input/input.hpp>
#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class Node;

	ALIAS(NODE) Ref<Node>;

	ALIAS(EntityID) entt::entity;

	ALIAS(EntityRegistry) entt::registry;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Node : public EventHandler
	{
		OBJECT_COMMON(Node, EventHandler);

		friend class SceneTree;

	protected:
		SceneTree *		m_tree		{};
		Node *			m_parent	{};
		Vector<NODE>	m_children	{};

		Node() noexcept;

	public:
		static constexpr size_t npos{ static_cast<size_t>(-1) };

		virtual ~Node() noexcept override;

		virtual void process(Duration const & dt);

		virtual void handle_event(Event const & ev) override;

	public:
		NODISCARD auto get_node(size_t const i) const -> NODE { return m_children[i]; }

		NODISCARD auto get_node_count() const noexcept -> size_t { return m_children.size(); }
		
		NODISCARD auto get_nodes() const noexcept -> Vector<NODE> const & { return m_children; }

		NODISCARD auto get_owner() const noexcept -> Node * { return m_parent; }

		NODISCARD auto get_tree() const noexcept -> SceneTree * { return m_tree; }

	public:
		NODE add_node(Node * value) noexcept { return (value && value->set_owner(this)) ? value : nullptr; }

		NODE add_node(NODE const & value) noexcept { return add_node(*value); }

		template <class T, class ... Args
		> NODE add_node(Args && ... args) noexcept { return add_node(memnew(T(FWD(args)...))); }

		void delete_node(size_t const i) { m_children.erase(m_children.begin() + i); }

		void detach_nodes();

		bool set_owner(Node * value);

		bool set_owner(NODE const & value) noexcept { return set_owner(*value); }

	public:
		NODISCARD size_t get_sibling_index() const noexcept;

		void set_sibling_index(size_t const i);

	public:
		NODISCARD bool is_owned_by(Node const * other, bool recursive = false) const noexcept;

		NODISCARD bool is_owned_by(NODE const & other, bool recursive = false) const noexcept { return is_owned_by(*other, recursive); }

		NODISCARD bool is_owner_of(Node const * other, bool recursive = false) const noexcept;

		NODISCARD bool is_owner_of(NODE const & other, bool recursive = false) const noexcept { return is_owner_of(*other, recursive); }

	public:
		template <class Fn = void(*)(NODE &)
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
				_for_nodes(m_children.begin(), m_children.end(), FWD(fn), recursive, reverse);
			}
			else
			{
				_for_nodes(m_children.rbegin(), m_children.rend(), FWD(fn), recursive, reverse);
			}
		}

		template <class Pr = bool(*)(NODE const &)
		> NODISCARD NODE find_node_if(Pr && pr, bool recursive = true, bool reverse = false) const noexcept
		{
			auto _find_node_if = [](auto first, auto last, auto pr, bool recursive, bool reverse) noexcept -> NODE
			{
				if (auto const it{ std::find_if(first, last, pr) }; it != last)
				{
					return (*it);
				}
				else if (recursive)
				{
					for (; first != last; ++first)
					{
						if (NODE found{ (*first)->find_node_if(pr, true, reverse) })
						{
							return found;
						}
					}
				}
				return nullptr;
			};

			if (!reverse)
			{
				return _find_node_if(m_children.begin(), m_children.end(), FWD(pr), recursive, reverse);
			}
			else
			{
				return _find_node_if(m_children.rbegin(), m_children.rend(), FWD(pr), recursive, reverse);
			}
		}

		template <class T
		> NODISCARD Ref<T> find_instance(bool recursive = true, bool reverse = false) const noexcept
		{
			return find_node_if([](NODE const & node) noexcept { return Ref<T>(node).is_valid(); }, recursive, reverse);
		}

		NODISCARD NODE find_node(Node const * value, bool recursive = true, bool reverse = false) const noexcept
		{
			return find_node_if([value](NODE const & node) noexcept { return node == value; }, recursive, reverse);
		}

		NODISCARD NODE find_node(NODE const & value, bool recursive = true, bool reverse = false) const noexcept
		{
			return find_node(*value, recursive, reverse);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
