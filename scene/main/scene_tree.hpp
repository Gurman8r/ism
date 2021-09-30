#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <scene/main/window.hpp>
#include <main/performance.hpp>

struct ImGuiContext;

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

	public:
		virtual ~SceneTree() override;

		explicit SceneTree(SceneSettings const & settings = {});

	public:
		virtual void initialize() override;

		virtual bool process(Duration const & delta_time) override;

		virtual void finalize() override;

	public:
		void on_runtime_update(Duration const & delta_time);

	protected:
		template <class T> void on_component_added(Node &, T &) {}

	public:
		NODISCARD auto get_name() const noexcept -> String const & { return m_name; }
		void set_name(String const & value) noexcept { if (m_name != value) { m_name = value; } }

		NODISCARD auto get_root() noexcept -> Window * { return m_root; }
		NODISCARD auto get_root() const noexcept -> Window const * { return m_root; }
		
		NODISCARD auto get_entt() const noexcept -> entt::registry & { return const_cast<entt::registry &>(m_entt); }
		NODISCARD auto get_imgui() const noexcept -> ImGuiContext * { return m_imgui; }

	private:
		friend class Node;
		friend class Entity;

		String			m_name	{};
		Window *		m_root	{};
		entt::registry	m_entt	{};
		ImGuiContext *	m_imgui	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
