#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <main/performance.hpp>

#include <entt/entt.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Node;
	class Window;

	ALIAS(EntityID) entt::entity;
	ALIAS(EntityRegistry) entt::registry;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene tree
	class ISM_API SceneTree : public MainLoop
	{
		OBJECT_COMMON(SceneTree, MainLoop);

		friend class Main;
		friend class Node;
		friend class Entity;

		static SceneTree * __singleton;

	public:
		SceneTree();
		virtual ~SceneTree() override;
		FORCE_INLINE static SceneTree * get_singleton() noexcept { return __singleton; }

		virtual void initialize() override;
		virtual bool process(Duration const & dt) override;
		virtual void finalize() override;

		void quit() { m_should_close = true; }

	public:
		auto get_root() const noexcept -> Window * { return m_root; }
		auto get_ecs() const noexcept -> EntityRegistry & { return const_cast<EntityRegistry &>(m_ecs); }
		auto get_elapsed_time() const noexcept -> Duration { return m_main_timer.get_elapsed_time(); }
		auto get_fps() const noexcept -> FPS_Tracker const & { return m_fps_tracker; }

	protected:
		template <class T> void on_component_added(class Entity &, T &) {}

	private:
		bool			m_initialized : 1, m_should_close : 1;
		Clock			m_main_timer{};
		FPS_Tracker		m_fps_tracker{};
		Window *		m_root{};
		EntityRegistry	m_ecs{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
