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
		friend class Node;
		friend class Entity;

		static SceneTree * singleton;

	public:
		explicit SceneTree(SceneSettings const & settings = {});

		virtual ~SceneTree() override;

		NODISCARD static SceneTree * get_singleton() noexcept { return singleton; }

	public:
		virtual void initialize() override;

		virtual bool process(Duration const & dt) override;

		virtual void finalize() override;

		virtual void handle_event(Event const & event) override;

	public:
		NODISCARD auto get_fps() const noexcept -> FrameRateTracker const & { return m_fps; }

		NODISCARD auto get_registry() const noexcept -> EntityRegistry & { return const_cast<EntityRegistry &>(m_entt); }

		NODISCARD auto get_root() const noexcept -> Ref<Window> { return m_root; }

		NODISCARD auto get_time() const noexcept -> Duration { return m_main_timer.elapsed(); }

	public:
		template <class Fn = void(*)(NODE &)
		> void for_nodes(Fn && fn, bool recursive = true, bool reverse = false) noexcept {
			if (!m_root) { return; }
			fn((NODE &)m_root);
			m_root->for_nodes(fn, recursive, reverse);
		}

	private:
		bool m_initialized{};

		Timer const m_main_timer{ true };

		FrameRateTracker m_fps{ 120 };

		Ref<Window> m_root{};

		EntityRegistry m_entt{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
