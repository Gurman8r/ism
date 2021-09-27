#include <runtime/scene/scene_tree.hpp>
#include <runtime/scene/components/all_components.hpp>

using namespace ism;

OBJECT_IMPL(SceneTree, t)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(SceneTree{ (String)STR(args[0]) }); };
};

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::~SceneTree()
	{
		if (m_root) { memdelete(m_root); m_root = nullptr; }
		if (m_window) { memdelete(m_window); m_window = nullptr; }
	}

	SceneTree::SceneTree(String const & name) : MainLoop{}
	{
		m_name = name.empty() ? "New Scene" : name;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		MainLoop::initialize();

		if (!m_window) m_window = Window::new_
		({
			"New Window",

			VideoMode{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },

			ContextSettings{ RendererAPI_OpenGL, 4, 6, RendererProfile_Compat, 24, 8, true, false },

			WindowHints_Default_Maximized
		});

		VERIFY(m_window);
	}

	bool SceneTree::process(Duration const & delta_time)
	{
		MainLoop::process(delta_time);

		return true;
	}

	void SceneTree::finalize()
	{
		if (m_root) { memdelete(m_root); m_root = nullptr; }
		if (m_window) { memdelete(m_window); m_window = nullptr; }

		MainLoop::finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::on_runtime_updateprocess(Duration const & delta_time)
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
		//	scr.instance->on_update(delta_time);
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