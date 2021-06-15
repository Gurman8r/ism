#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <scene/main/window.hpp>

// scene
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Entity;

	class ISM_API SceneTree : public MainLoop
	{
		ISM_SUPER(SceneTree, MainLoop);

	public:
		virtual ~SceneTree() override;

		explicit SceneTree(String const & name = "New Scene");

		NON_COPYABLE(SceneTree);

		MOVABLE(SceneTree);

	public:
		void on_runtime_update(Duration dt);

	protected:
		template <class T> void on_component_added(Entity &, T &) {}

	public:
		NODISCARD auto get_registry() const noexcept { return const_cast<entt::registry *>(&m_reg); }

		NODISCARD auto get_root() const noexcept -> Window * { return m_root; }

		NODISCARD auto get_name() const noexcept -> String const & { return m_name; }

		void set_name(String const & value) noexcept { if (m_name != value) { m_name = value; } }

	private:
		friend class Node;

		String			m_name	{}; // name
		Window *		m_root	{}; // root
		entt::registry	m_reg	{}; // registry
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_