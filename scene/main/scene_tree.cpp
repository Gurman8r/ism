#include <scene/main/scene_tree.hpp>
#include <scene/components/components.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::~SceneTree()
	{
		if (m_root) { memdelete(m_root); m_root = nullptr; }
	}

	SceneTree::SceneTree(String const & name)
	{
		m_name = name.empty() ? "New Scene" : name;

		m_root = Window::new_
		({
			this, nullptr,

			"New Window",

			VideoMode{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },

			ContextSettings{ ContextAPI_OpenGL, 4, 6, ContextProfile_Compat, 24, 8, true, false },

			WindowHints_Default_Maximized
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		MainLoop::initialize();
	}

	bool SceneTree::iteration(Duration delta_time)
	{
		if (!MainLoop::iteration(delta_time)) { return false; }

		return m_root && m_root->is_open();
	}

	void SceneTree::finalize()
	{
		
		MainLoop::finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::on_runtime_update(Duration dt)
	{
		if (!m_root) { return; }

		//m_reg.view<ScriptComponent>().each([&](auto e, ScriptComponent & scr)
		//{
		//	if (!scr.instance)
		//	{
		//		scr.instance = scr.create_instance();
		//	
		//		scr.instance->on_create();
		//	}
		//	
		//	scr.instance->on_update(dt);
		//});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <> void SceneTree::on_component_added<CameraComponent>(Node & e, CameraComponent & c)
	{
	}
	
	template <> void SceneTree::on_component_added<ScriptComponent>(Node & e, ScriptComponent & c)
	{
	}

	template <> void SceneTree::on_component_added<TagComponent>(Node & e, TagComponent & c)
	{
	}

	template <> void SceneTree::on_component_added<TransformComponent>(Node & e, TransformComponent & c)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}