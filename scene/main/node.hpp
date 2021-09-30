#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/input/input.hpp>
#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class ISM_API Node : public Object
	{
		OBJECT_COMMON(Node, Object);
		
		friend class SceneTree;

	protected:
		SceneTree *			m_scene		{}; // scene
		Node *				m_parent	{}; // parent
		Vector<Ref<Node>>	m_children	{}; // children

	public:
		using iterator					= typename Vector<Ref<Node>>::iterator;
		using const_iterator			= typename Vector<Ref<Node>>::const_iterator;
		using reverse_iterator			= typename Vector<Ref<Node>>::reverse_iterator;
		using const_reverse_iterator	= typename Vector<Ref<Node>>::const_reverse_iterator;

	protected:
		Node(SceneTree * scene = nullptr, Node * parent = nullptr) noexcept : Object{}
		{
			if (scene && parent)
			{
				m_scene = scene;
				m_parent = parent;
			}
			else if (scene && !parent)
			{
				m_scene = scene;
				m_parent = nullptr;
			}
			else if (!scene && parent)
			{
				m_scene = parent->m_scene;
				m_parent = parent;
			}
		}

	public:
		NODISCARD auto get_child(size_t i) const noexcept -> Ref<Node> { return (i < get_child_count()) ? m_children[i] : nullptr; }

		NODISCARD auto get_child_count() const noexcept -> size_t { return m_children.size(); }

		NODISCARD auto get_children() const noexcept -> Vector<Ref<Node>> const & { return m_children; }

		NODISCARD auto get_parent() const noexcept -> Node * { return m_parent; }

		NODISCARD auto get_scene() const noexcept -> SceneTree * { return m_scene; }

		NODISCARD auto get_sibling(size_t i) const noexcept -> Ref<Node> { return m_parent ? m_parent->get_child(i) : nullptr; }

		NODISCARD auto get_sibling_count() const noexcept -> size_t { return !m_parent ? m_parent->get_child_count() : 0; }

		NODISCARD auto get_sibling_index() const noexcept -> size_t
		{
			return m_parent
				? std::distance(m_parent->m_children.begin(), std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(), [&
				](auto const & child) { return this == child; }))
				: static_cast<size_t>(-1);
		}

		template <class Pr
		> NODISCARD auto get_node_if(Pr && pr, bool recursive = true) const -> Ref<Node>
		{
			if (auto const it{ std::find_if(m_children.begin(), m_children.end(), FWD(pr)) }; it != m_children.end())
			{
				return (*it);
			}
			else if (recursive)
			{
				for (auto const & child : m_children)
				{
					if (auto const found{ child->get_node_if(FWD(pr)) })
					{
						return found;
					}
				}
			}
			return nullptr;
		}

		NODISCARD auto get_node(Ref<Node> const & value, bool recursive = true) const -> Ref<Node>
		{
			return get_node_if([&](auto const & child) noexcept
			{
				return child == value;
			}
			, recursive);
		}

		NODISCARD bool is_child_of(Ref<Node> const & other) const noexcept
		{
			return (m_parent && other && (this != other)) && ((m_parent == other) || other->get_node_if([&](auto child) noexcept
			{
				return is_child_of(child);
			}));
		}

		NODISCARD bool is_parent_of(Ref<Node> const & other) const noexcept
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
			m_children.clear();
		}

		void delete_child(size_t i)
		{
			if (i < m_children.size())
			{
				m_children.erase(m_children.begin() + (ptrdiff_t)i);
			}
		}

		void detatch_children()
		{
			if (m_parent)
			{
				for (auto const & child : m_children)
				{
					child->m_parent = m_parent;

					m_parent->m_children.push_back(child);
				}
			}
			else
			{
				for (auto const & child : m_children)
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

			auto & children{ m_parent->m_children };

			children.erase(children.begin() + get_sibling_index());

			children.insert(children.begin() + i, this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
