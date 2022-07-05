#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <main/performance.hpp>
#include <scene/main/window.hpp>

// scene
namespace ism
{
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
		auto get_ecs() const noexcept -> EntityRegistry & { return const_cast<EntityRegistry &>(m_entt); }

		auto get_fps() const noexcept -> FrameRateTracker const & { return m_fps; }

		auto get_root() const noexcept -> Window * { return m_root; }

		auto get_elapsed_time() const noexcept -> Duration { return m_main_timer.elapsed(); }

	public:
		template <class Fn = void(*)(Ref<Node> &)
		> void each_child(Fn && fn, bool recursive = true, bool reverse = false) noexcept {
			if (!m_root) { return; }
			fn((Ref<Node> &)m_root);
			m_root->each_child(fn, recursive, reverse);
		}

	protected:
		template <class T> void on_component_added(class Entity &, T &) {}

	private:
		bool m_initialized{};

		Timer const m_main_timer{ true };

		FrameRateTracker m_fps{ 120 };

		Window * m_root{};

		EntityRegistry m_entt{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SCENE_TREE (ism::SceneTree::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
