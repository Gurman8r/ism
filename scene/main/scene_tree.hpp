#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <scene/main/node.hpp>
#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Entity;

	class ISM_API SceneTree : public MainLoop
	{
		ISM_SUPER_CLASS(SceneTree, MainLoop);

	public:
		using allocator_type = typename PolymorphicAllocator<byte>;

	public:
		virtual ~SceneTree() override = default;

		SceneTree(String const & name, allocator_type alloc = {}) noexcept
			: m_name{ name.empty() ? "New Scene" : name, alloc }
			, m_reg	{}
			, m_root{ memnew(Node(name, this, alloc)) }
		{
		}

		SceneTree(SceneTree && other, allocator_type alloc = {}) noexcept
			: m_name{ alloc }
			, m_reg	{}
			, m_root{}
		{
			this->swap(std::move(other));
		}

		SceneTree & operator=(SceneTree && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(SceneTree & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_name, other.m_name);
				std::swap(m_reg, other.m_reg);
				std::swap(m_root, other.m_root);
			}
		}

	public:
		NODISCARD auto get_root() const noexcept -> Ref<Node> const & { return m_root; }

		NODISCARD auto get_name() const noexcept -> String const & { return m_name; }

		void set_name(String const & value) noexcept { if (m_name != value) { m_name = value; } }

	public:
		void on_runtime_update(Duration dt);

	protected:
		template <class T> void on_component_added(Entity &, T &) {}

	private:
		friend class Node;
		friend class Entity;

		String			m_name	; // name
		entt::registry	m_reg	; // registry
		Ref<Node>		m_root	; // root Node
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
