#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Entity final
	{
	public:
		Entity() noexcept
			: m_tree	{}
			, m_handle	{}
		{
		}

		Entity(SceneTree const * tree, entt::entity handle) noexcept
			: m_tree	{ const_cast<SceneTree *>(tree) }
			, m_handle	{ handle }
		{
		}

		Entity(SceneTree const * tree) noexcept
			: m_tree	{ const_cast<SceneTree *>(tree) }
			, m_handle	{ m_tree ? m_tree->m_reg.create() : entt::null }
		{
		}

		Entity(Entity const & other) noexcept
			: m_tree	{ other.m_tree }
			, m_handle	{ other.m_handle }
		{
		}

		Entity(Entity && other) noexcept
			: m_tree	{}
			, m_handle	{}
		{
			this->swap(std::move(other));
		}

		Entity & operator=(Entity const & other)
		{
			Entity temp{ other };
			this->swap(temp);
			return (*this);
		}

		Entity & operator=(Entity && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(Entity & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_tree, other.m_tree);
				std::swap(m_handle, other.m_handle);
			}
		}

	public:
		template <class T, class ... Args
		> auto add(Args && ... args) noexcept -> T &
		{
			T & c{ m_tree->m_reg.emplace<T>(m_handle, FWD(args)...) };
			m_tree->on_component_added<T>(*this, c);
			return c;
		}

		template <class ... T
		> NODISCARD decltype(auto) get() noexcept
		{
			VERIFY(valid());
			return m_tree->m_reg.get<T...>(m_handle);
		}

		template <class ... T
		> NODISCARD bool has() const noexcept
		{
			VERIFY(valid());
			return m_tree->m_reg.has<T...>(m_handle);
		}

		template <class ... T
		> void remove() noexcept
		{
			VERIFY(valid());
			m_tree->m_reg.remove<T...>(m_handle);
		}

	public:
		NODISCARD operator bool() const noexcept { return this->valid(); }

		NODISCARD operator entt::entity() const noexcept { return m_handle; }

		NODISCARD auto get_handle() const noexcept -> entt::entity { return m_handle; }

		NODISCARD auto get_tree() const noexcept -> SceneTree * { return m_tree; }

		NODISCARD bool valid() const noexcept { return m_tree && m_tree->m_reg.valid(m_handle); }

	private:
		friend SceneTree;

		SceneTree * m_tree; // tree

		entt::entity m_handle; // handle
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
