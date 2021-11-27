#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <scene/main/window.hpp>
#include <main/performance.hpp>

// scene
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	struct NODISCARD SceneSettings final
	{
		DEFAULT_COPYABLE_MOVABLE(SceneSettings);

		String name{ "New Scene" };
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API SceneTree : public MainLoop
	{
		OBJECT_COMMON(SceneTree, MainLoop);

		friend class Main;
		friend class Entity;

		static SceneTree * singleton;

	public:
		SceneTree(SceneSettings const & settings = {});

		virtual ~SceneTree() override;

		NODISCARD static SceneTree * get_singleton() noexcept { return singleton; }

	public:
		virtual void initialize() override;

		virtual bool process(Duration const & delta_time) override;

		virtual void finalize() override;

	public:
		NODISCARD Ref<Window> get_root() const noexcept { return m_root; }

		NODISCARD Duration get_time() const noexcept { return m_main_timer.elapsed(); }

		NODISCARD float_t get_framerate() const noexcept { return m_fps.value; }

	public:
		template <class Fn = void(*)(NODE &)
		> void for_nodes(Fn && fn, bool recursive = true, bool reverse = false)
		{
			if (!m_root) { return; }
			fn((NODE &)m_root);
			m_root->for_nodes(fn, recursive, reverse);
		}

	protected:
		template <class T> void on_component_added(Entity &, T &) {}
		
	private:
		bool m_initialized{};

		Ref<Window> m_root{};

		Timer const m_main_timer{ true };

		FPS_Tracker m_fps{ 120 };

		entt::registry m_entt{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
