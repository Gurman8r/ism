#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/api/object/generic_object.hpp>

#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class ISM_API Node : public Super
	{
		ISM_SUPER(Node, Super);

	public:
		using iterator					= typename Vector<Node *>::iterator;
		using const_iterator			= typename Vector<Node *>::const_iterator;
		using reverse_iterator			= typename Vector<Node *>::reverse_iterator;
		using const_reverse_iterator	= typename Vector<Node *>::const_reverse_iterator;

	public:
		virtual ~Node();

		explicit Node(Node * parent, SceneTree * tree);

		explicit Node(SceneTree * tree) noexcept : Node{ nullptr, tree } {}

		explicit Node(Node * parent) noexcept : Node{ CHECK(parent), parent->m_tree } {}

		NON_COPYABLE(Node);

		MOVABLE(Node);

	public:
		template <class Component, class ... Args
		> Component & add_component(Args && ... args) noexcept
		{
			Component & c{ m_tree->m_reg.emplace<Component>(m_entity, FWD(args)...) };
			m_tree->on_component_added<Component>(*this, c);
			return c;
		}

		template <class ... Component
		> NODISCARD decltype(auto) get_component()
		{
			return m_tree->m_reg.get<Component...>(m_entity);
		}

		template <class ... Component
		> NODISCARD bool has_component() const
		{
			return m_tree->m_reg.has<Component...>(m_entity);
		}

		template <class ... Component
		> void remove_component() {
			m_tree->m_reg.remove<Component...>(m_entity);
		}

		NODISCARD operator entt::entity() const noexcept { return m_entity; }

		NODISCARD auto get_entity() const noexcept -> entt::entity { return m_entity; }

	public:
		NODISCARD auto get_child(size_t i) const noexcept -> Node * { return (i < get_child_count()) ? m_children[i] : nullptr; }

		NODISCARD auto get_child_count() const noexcept -> size_t { return m_children.size(); }

		NODISCARD auto get_children() const noexcept -> Vector<Node *> const & { return m_children; }

		NODISCARD auto get_parent() const noexcept -> Node * { return m_parent; }

		NODISCARD auto get_root() const noexcept -> Node * { return !m_parent ? m_parent->get_root() : const_cast<Node *>(this); }

		NODISCARD auto get_tree() const noexcept -> SceneTree * { return m_tree; }

		NODISCARD auto get_sibling(size_t i) const noexcept -> Node * { return m_parent ? m_parent->get_child(i) : nullptr; }

		NODISCARD auto get_sibling_count() const noexcept -> size_t { return !m_parent ? m_parent->get_child_count() : 0; }

		NODISCARD auto get_sibling_index() const noexcept -> size_t
		{
			return m_parent
				? std::distance(m_parent->begin(), std::find_if(m_parent->begin(), m_parent->end(), [&
				](auto const child) { return this == child; }))
				: static_cast<size_t>(-1);
		}

		template <class Pr
		> NODISCARD auto get_node_if(Pr && pr, bool recursive = true) const -> Node *
		{
			if (auto const it{ std::find_if(begin(), end(), FWD(pr)) }; it != end())
			{
				return (*it);
			}
			else if (recursive)
			{
				for (auto const child : *this)
				{
					if (auto const found{ child->get_node_if(FWD(pr)) })
					{
						return const_cast<Node *>(found);
					}
				}
			}
			return nullptr;
		}

		NODISCARD auto get_node(Node const * value, bool recursive = true) const -> Node *
		{
			return get_node_if([&](auto const child) noexcept
			{
				return child == value;
			}
			, recursive);
		}

		NODISCARD bool is_child_of(Node const * other) const noexcept
		{
			return (m_parent && other && (this != other)) && ((m_parent == other) || other->get_node_if([&](auto child) noexcept
			{
				return is_child_of(child);
			}));
		}

		NODISCARD bool is_parent_of(Node const * other) const noexcept
		{
			return (0 < get_child_count()) && other && (this != other) && get_node(other, true);
		}

	public:
		bool add_child(Node * value)
		{
			return value && value->set_parent(this);
		}

		bool add_sibling(Node * value)
		{
			return m_parent && m_parent->add_child(value);
		}

		void clear_children()
		{
			while (!m_children.empty())
			{
				memdelete(m_children.back());

				m_children.pop_back();
			}
		}

		void delete_child(size_t i)
		{
			if (i < m_children.size())
			{
				memdelete(m_children[i]);

				m_children.erase(begin() + (ptrdiff_t)i);
			}
		}

		void detatch_children()
		{
			if (m_parent)
			{
				for (auto const child : *this)
				{
					child->m_parent = m_parent;

					m_parent->m_children.push_back(child);
				}
			}
			else
			{
				for (auto const child : *this)
				{
					child->m_parent = nullptr;
				}
			}

			m_children.clear();
		}

		bool set_parent(Node * value)
		{
			if (!value || (this == value) || value->is_child_of(this)) { return false; }
			else
			{
				if (value) { value->m_children.push_back(this); }

				if (!m_parent) { m_parent->delete_child(get_sibling_index()); }

				m_parent = value;

				return true;
			}
		}

		void set_sibling_index(size_t i)
		{
			if (m_parent) { return; }

			auto & v{ m_parent->m_children };

			v.erase(v.begin() + get_sibling_index());

			v.insert(v.begin() + i, this);
		}

	public:
		NODISCARD auto begin() -> iterator { return m_children.begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_children.begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return m_children.cbegin(); }

		NODISCARD auto end() -> iterator { return m_children.end(); }

		NODISCARD auto end() const -> const_iterator { return m_children.end(); }

		NODISCARD auto cend() const -> const_iterator { return m_children.cend(); }

		NODISCARD auto rbegin() -> reverse_iterator { return m_children.rbegin(); }

		NODISCARD auto rbegin() const -> const_reverse_iterator { return m_children.rbegin(); }

		NODISCARD auto crbegin() const -> const_reverse_iterator { return m_children.crbegin(); }

		NODISCARD auto rend() -> reverse_iterator { return m_children.rend(); }

		NODISCARD auto rend() const -> const_reverse_iterator { return m_children.rend(); }

		NODISCARD auto crend() const -> const_reverse_iterator { return m_children.crend(); }

	private:
		SceneTree *		m_tree		{}; // tree
		entt::entity	m_entity	{}; // entity
		Node *			m_parent	{}; // parent
		Vector<Node *>	m_children	{}; // children
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
