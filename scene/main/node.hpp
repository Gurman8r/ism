#ifndef _ISM_NODE_HPP_
#define _ISM_NODE_HPP_

#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;

	class Node;

	ALIAS(NODE) Ref<Node>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Node : public EventHandler
	{
		OBJECT_COMMON(Node, EventHandler);

		friend class SceneTree;

	protected:
		SceneTree *		m_tree	{};
		Node *			m_owner	{};
		Vector<NODE>	m_nodes	{};

		Node() noexcept;

	public:
		static constexpr size_t npos{ static_cast<size_t>(-1) };

		virtual ~Node() noexcept override;

		virtual void process(Duration const & dt);

		virtual void handle_event(Event const & event) override;

	public:
		NODISCARD auto get_node(size_t const i) const -> NODE { return m_nodes[i]; }

		NODISCARD auto get_node_count() const noexcept -> size_t { return m_nodes.size(); }

		NODISCARD auto get_owner() const noexcept -> Node * { return m_owner; }

		NODISCARD auto get_tree() const noexcept -> SceneTree * { return m_tree; }

		NODE add_node(Node * value) noexcept { return (value && value->set_owner(this)) ? value : nullptr; }

		NODE add_node(NODE const & value) noexcept { return add_node(*value); }

		template <class T, class ... Args
		> NODE add_node(Args && ... args) noexcept { return add_node(memnew(T(FWD(args)...))); }

		void delete_node(size_t const i) { m_nodes.erase(m_nodes.begin() + i); }

		void detach_nodes();

		bool set_owner(Node * value);

		bool set_owner(NODE const & value) noexcept { return set_owner(*value); }

		NODISCARD size_t get_subindex() const noexcept;

		void set_subindex(size_t const i);

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
				_for_nodes(m_nodes.begin(), m_nodes.end(), FWD(fn), recursive, reverse);
			}
			else
			{
				_for_nodes(m_nodes.rbegin(), m_nodes.rend(), FWD(fn), recursive, reverse);
			}
		}

	public:
		template <class Pr = bool(*)(NODE const &)
		> NODISCARD NODE find_if(Pr && pr, bool recursive = true, bool reverse = false) const noexcept
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
		> NODISCARD Ref<T> find(bool recursive = true, bool reverse = false) const noexcept
		{
			return find_if([](NODE const & node) noexcept { return Ref<T>(node).is_valid(); }, recursive, reverse);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NODE_HPP_
