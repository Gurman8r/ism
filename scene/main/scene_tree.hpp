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
		friend class Node;
		friend class Entity;

		static SceneTree * singleton;

		Window * m_root{};

		bool m_initialized{};

		entt::registry m_entt{};

	public:
		SceneTree(SceneSettings const & settings = {});

		virtual ~SceneTree() override;

		NODISCARD static SceneTree * get_singleton() noexcept { return singleton; }

	public:
		virtual void initialize() override;

		virtual bool process(Duration const & delta_time) override;

		virtual void finalize() override;

	public:
		NODISCARD auto get_root() const noexcept -> Window * const { return const_cast<Window *>(m_root); }

	protected:
		template <class T> void on_component_added(Node &, T &) {}
		
	private:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
