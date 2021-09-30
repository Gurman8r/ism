#include <scene/main/scene_tree.hpp>
#include <scene/components/all_components.hpp>
#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(SceneTree, t) {};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SceneTree::SceneTree(SceneSettings const & settings) : MainLoop{}
{
	m_name = settings.name.empty() ? "New Scene" : settings.name;
}

SceneTree::~SceneTree()
{
	if (m_root) { memdelete(m_root); m_root = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SceneTree::initialize()
{
	MainLoop::initialize();

	m_root = Window::new_
	({
		"New Window",

		VideoMode{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },

		ContextSettings{ RendererAPI_OpenGL, 4, 6, RendererProfile_Compat, 24, 8, true, false },

		WindowHints_Default_Maximized
	});

	m_root->m_scene = this;

	get_rendering_device().initialize();

	VERIFY(m_imgui = ImGui::CreateContext());
	m_imgui->IO.LogFilename = nullptr;
	m_imgui->IO.IniFilename = nullptr;
	m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	VERIFY(ImGui_Init(m_root->get_window_id()));
}

bool SceneTree::process(Duration const & delta_time)
{
	MainLoop::process(delta_time);

	return true;
}

void SceneTree::finalize()
{
	ImGui::DestroyContext(m_imgui);

	if (m_root) { memdelete(m_root); m_root = nullptr; }

	MainLoop::finalize();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SceneTree::on_runtime_update(Duration const & delta_time)
{
	if (!m_root) { return; }

	//m_registry.view<ScriptComponent>().each([&](auto e, ScriptComponent & scr)
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