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

	class ISM_API Node : public Object
	{
		OBJECT_COMMON(Node, Object);

		friend class SceneTree;

		SceneTree *		m_tree	{};
		Node *			m_owner	{};
		Vector<NODE>	m_nodes	{};

	protected:
		Node() noexcept : Object{} {}

	public:
		static constexpr size_t npos{ static_cast<size_t>(-1) };

		virtual ~Node() override;

		virtual void initialize() {}
		
		virtual void finalize() {}

		virtual void begin_step() {}
		
		virtual void step(Duration const &) {}
		
		virtual void end_step() {}

	public:
		NODISCARD SceneTree * get_tree() const noexcept { return m_tree; }

		NODISCARD Node * get_owner() const noexcept { return m_owner; }

		NODISCARD NODE get_node(size_t const i) const noexcept { VERIFY(i < get_node_count()); return m_nodes[i]; }

		NODISCARD size_t get_node_count() const noexcept { return m_nodes.size(); }

		NODISCARD size_t get_sibling_index() const noexcept
		{
			if (m_owner)
			{
				for (size_t i = 0; i < m_owner->m_nodes.size(); ++i)
				{
					if (m_owner->m_nodes[i] == this)
					{
						return i;
					}
				}
			}
			return npos;
		}

	public:
		template <class T, class ... Args
		> NODE add_node(Args && ... args) noexcept { return add_node(memnew(T(FWD(args)...))); }

		NODE add_node(Node * value) noexcept { return (value && value->set_owner(this)) ? value : nullptr; }

		NODE add_node(NODE const & value) noexcept { return add_node(value.ptr()); }

		void delete_node(size_t const i);

		void detach_nodes();

		bool set_owner(Node * value);

		bool set_owner(NODE const & value) noexcept { return set_owner(value.ptr()); }

		void set_sibling_index(size_t const i);

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
				_for_nodes(m_nodes.begin(), m_nodes.end(), FWD(fn), recursive, reverse);
			}
			else
			{
				_for_nodes(m_nodes.rbegin(), m_nodes.rend(), FWD(fn), recursive, reverse);
			}
		}

	public:
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
				return _find_node_if(m_nodes.begin(), m_nodes.end(), FWD(pr), recursive, reverse);
			}
			else
			{
				return _find_node_if(m_nodes.rbegin(), m_nodes.rend(), FWD(pr), recursive, reverse);
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
			return find_node(value.ptr(), recursive, reverse);
		}

	public:
		NODISCARD bool is_owner_of(Node const * other, bool recursive = false) const noexcept
		{
			if (!other || (this == other)) { return false; }
			else if (this == other->m_owner) { return true; }
			else if (recursive)
			{
				for (NODE const & node : m_nodes)
				{
					if (node->is_owner_of(other, true))
					{
						return true;
					}
				}
			}
			return false;
		}

		NODISCARD bool is_owner_of(NODE const & other, bool recursive = false) const noexcept
		{
			return is_owner_of(other.ptr(), recursive);
		}

		NODISCARD bool is_owned_by(Node const * other, bool recursive = false) const noexcept
		{
			if (!m_owner || !other || (this == other)) { return false; }
			else if (m_owner == other) { return true; }
			else if (recursive)
			{
				Node * it{ m_owner->m_owner };
				while (it)
				{
					if (it == other) {
						return true;
					}
					it = it->m_owner;
				}
			}
			return false;
		}
		
		NODISCARD bool is_owned_by(NODE const & other, bool recursive = false) const noexcept
		{
			return is_owned_by(other.ptr(), recursive);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
