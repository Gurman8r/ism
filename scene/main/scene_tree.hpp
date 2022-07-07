#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/os/time.hpp>
#include <main/performance.hpp>
#include <scene/main/window.hpp>

// scene
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(EntityID) entt::entity;

	ALIAS(EntityRegistry) entt::registry;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	struct SceneSettings final
	{
		String name{ "New Scene" };
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API SceneTree : public MainLoop
	{
		OBJECT_COMMON(SceneTree, MainLoop);

		friend class Main;
		friend class Node;
		friend class Entity;

		static SceneTree * g_singleton;

	public:
		explicit SceneTree(SceneSettings const & settings = {});
		virtual ~SceneTree() override;
		FORCE_INLINE static SceneTree * get_singleton() noexcept { return g_singleton; }

	public:
		virtual void initialize() override;
		virtual bool process(Duration const & dt) override;
		virtual void finalize() override;
		virtual void handle_event(Event const & event) override;

	public:
		auto get_root() const noexcept -> Window * { return m_root; }
		auto get_ecs() const noexcept -> EntityRegistry & { return const_cast<EntityRegistry &>(m_ecs); }
		auto get_elapsed_time() const noexcept -> Duration { return m_main_timer.get_elapsed_time(); }
		auto get_fps() const noexcept -> FPS_Tracker const & { return m_fps; }

	protected:
		template <class T> void on_component_added(class Entity &, T &) {}

	private:
		bool			m_initialized{};
		Clock			m_main_timer{};
		FPS_Tracker		m_fps{};
		Window *		m_root{};
		EntityRegistry	m_ecs{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SCENE_TREE (ism::SceneTree::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
