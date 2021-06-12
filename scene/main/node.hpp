#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/api/reference.hpp>

#include <memory>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class ISM_API Node : public Reference
	{
		ISM_SUPER_CLASS(Node, Reference);

	public:
		using allocator_type			= typename PolymorphicAllocator<byte>;
		using iterator					= typename Vector<Ref<Node>>::iterator;
		using const_iterator			= typename Vector<Ref<Node>>::const_iterator;
		using reverse_iterator			= typename Vector<Ref<Node>>::reverse_iterator;
		using const_reverse_iterator	= typename Vector<Ref<Node>>::const_reverse_iterator;

	public:
		virtual ~Node() override = default;

		Node(allocator_type alloc = {}) noexcept
			: m_name	{ "New Node", alloc }
			, m_tree	{}
			, m_parent	{}
			, m_children{ alloc }
		{
		}

		Node(String const & name, SceneTree * tree, Ref<Node> const & parent, allocator_type alloc = {})
			: m_name	{ name.empty() ? "New Node" : name, alloc }
			, m_tree	{ tree }
			, m_parent	{ parent.ptr() }
			, m_children{ alloc }
		{
		}

		Node(String const & name, SceneTree * tree, allocator_type alloc = {})
			: m_name	{ name.empty() ? "New Node" : name, alloc }
			, m_tree	{ tree }
			, m_parent	{}
			, m_children{ alloc }
		{
		}

		Node(String const & name, Ref<Node> const & parent, allocator_type alloc = {})
			: m_name	{ name.empty() ? "New Node" : name, alloc }
			, m_tree	{ parent ? parent->get_tree() : nullptr }
			, m_parent	{ parent.ptr() }
			, m_children{ alloc }
		{
		}

		Node(Node && other, allocator_type alloc = {}) noexcept
			: m_name	{ alloc }
			, m_tree	{}
			, m_parent	{}
			, m_children{ alloc }
		{
			this->swap(std::move(other));
		}

		Node & operator=(Node && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(Node & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_name, other.m_name);
				std::swap(m_tree, other.m_tree);
				std::swap(m_parent, other.m_parent);
				std::swap(m_children, other.m_children);
			}
		}

	public:
		NODISCARD auto get_name() const noexcept -> String const & { return m_name; }

		void set_name(String const & value) noexcept { if (m_name != value) { m_name = value; } }

	public:
		NODISCARD auto get_child(size_t i) const noexcept -> Ref<Node> const &
		{
			return m_children[i];
		}

		NODISCARD auto get_child_count() const noexcept -> size_t
		{
			return m_children.size();
		}

		NODISCARD auto get_children() const noexcept -> Vector<Ref<Node>> const &
		{
			return m_children;
		}

		NODISCARD auto get_parent() const noexcept -> Node *
		{
			return m_parent;
		}

		NODISCARD auto get_root() const noexcept -> Ref<Node>
		{
			return !m_parent ? get_parent()->get_root() : const_cast<Node *>(this);
		}

		NODISCARD auto get_sibling_count() const noexcept -> size_t
		{
			return !m_parent ? get_parent()->get_child_count() : 0;
		}

		NODISCARD auto get_sibling_index() const noexcept -> size_t
		{
			if (m_parent) { return static_cast<size_t>(-1); }
			else
			{
				auto const & v{ get_parent()->m_children };
				return (size_t)std::distance(v.begin(), std::find_if(v.begin(), v.end(), [&
				](Ref<Node> const & e) noexcept { return this == e.ptr(); }));
			}
		}

		NODISCARD auto get_tree() const noexcept -> SceneTree *
		{
			return m_tree;
		}

		NODISCARD bool is_child_of(Ref<Node> const & other) const noexcept
		{
			if (m_parent || !other || (this == other.ptr())) { return false; }
			else
			{
				return (get_parent() == other.ptr()) || other->find_node_if([&
				](Ref<Node> const & child) noexcept
				{
					return is_child_of(child);
				});
			}
		}

		NODISCARD bool is_parent_of(Ref<Node> const & other) const
		{
			return false;
		}

	public:
		bool add_child(Ref<Node> const & value)
		{
			return value && value->set_parent(this);
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
			for (Ref<Node> const & child : m_children)
			{
				child->m_parent = m_parent;

				if (!m_parent) { m_parent->m_children.push_back(child); }
			}

			m_children.clear();
		}

		auto new_child(String const & name) -> Ref<Node> &
		{
			return m_children.emplace_back(memnew(Node(name, m_tree, this)));
		}

		bool set_parent(Ref<Node> const & value)
		{
			if (!value || (this == value.ptr()) || value->is_child_of(this))
			{
				return false;
			}
			else
			{
				if (value) { value->m_children.push_back(this); }

				if (!m_parent) { m_parent->delete_child(get_sibling_index()); }

				m_parent = value.ptr();

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
		template <class Pr
		> auto find_node_if(Pr && pr, bool recursive = true) -> Ref<Node>
		{
			if (auto const it{ std::find_if(m_children.begin(), m_children.end(), FWD(pr)) }
			; it != m_children.end())
			{
				return (*it);
			}
			else if (recursive)
			{
				for (Ref<Node> const & child : m_children)
				{
					if (Ref<Node> found{ child->find_node_if(FWD(pr)) })
					{
						return found;
					}
				}
			}
			return nullptr;
		}

		template <class Pr
		> auto find_node_if(Pr && pr, bool recursive = true) const -> Ref<Node>
		{
			if (auto const it{ std::find_if(m_children.begin(), m_children.end(), FWD(pr)) }
			; it != m_children.end())
			{
				return (*it);
			}
			else if (recursive)
			{
				for (Ref<Node> const & child : m_children)
				{
					if (Ref<Node> found{ child->find_node_if(FWD(pr)) })
					{
						return found;
					}
				}
			}
			return nullptr;
		}

		auto find_node(Node const * value, bool recursive = true) -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e.ptr() == value; }
			, recursive);
		}

		auto find_node(Node const * value, bool recursive = true) const -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e.ptr() == value; }
			, recursive);
		}

		auto find_node(Ref<Node> const & value, bool recursive = true) const -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e == value; }
			, recursive);
		}

		auto find_node(Ref<Node> const & value, bool recursive = true) -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e == value; }
			, recursive);
		}

		auto find_node(String const & value, bool recursive = true) -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e && e->get_name() == value; }
			, recursive);
		}

		auto find_node(String const & value, bool recursive = true) const -> Ref<Node>
		{
			return this->find_node_if([&
			](Ref<Node> const & e) noexcept { return e && e->get_name() == value; }
			, recursive);
		}

	private:
		friend SceneTree;

		String				m_name		; // name
		SceneTree *			m_tree		; // tree
		Node *				m_parent	; // parent
		Vector<Ref<Node>>	m_children	; // children
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
