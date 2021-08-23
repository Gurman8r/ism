#include <scene/main/scene_tree.hpp>
#include <scene/components/components.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(SceneTree, t, "scene_tree")
{
	t.tp_base = typeof<MainLoop>();

	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(SceneTree{ (String)STR(args[0]) }); };
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::~SceneTree()
	{
		if (m_root) { memdelete(m_root); m_root = nullptr; }
	}

	SceneTree::SceneTree(String const & name) : SceneTree{ get_class() }
	{
		m_name = name.empty() ? "New Scene" : name;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		MainLoop::initialize();

		if (!m_root) m_root = Window::new_(this, nullptr,
		{
			"New Window",

			VideoMode{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },

			ContextSettings{ ContextAPI_OpenGL, 4, 6, ContextProfile_Compat, 24, 8, true, false },

			WindowHints_Default_Maximized
		});

		VERIFY(m_root);
	}

	bool SceneTree::process(Duration delta_time)
	{
		MainLoop::process(delta_time);

		return true;
	}

	void SceneTree::finalize()
	{
		if (m_root) { memdelete(m_root); m_root = nullptr; }

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