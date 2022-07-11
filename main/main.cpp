/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/imgui.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if TOOLS_ENABLED
#include <editor/register_editor_types.hpp>
#endif

#if SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define DISPLAY_SERVER_DEFAULT DisplayServerWindows
#endif

#include <servers/rendering/rendering_server_default.hpp>

#include <servers/audio_server.hpp>
#include <servers/text_server.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

static bool editor{ true };

int32_t Main::g_iterating{};

static Internals *			g_internals{};
static Input *				g_input{};
static AudioServer *		g_audio{};
static DisplayServer *		g_display{};
static RenderingServer *	g_renderer{};
static ImGuiContext *		g_imgui{};
static TextServer *			g_text{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ Main::setup(cstring exepath, int32_t argc, char * argv[])
{
	OS::get_singleton()->initialize();
	
	g_internals = memnew(Internals);

	register_core_types();
	
	register_core_driver_types();
	
	register_core_settings();

	//preregister_module_types();
	
	preregister_server_types();
	
	register_server_types();
	
	register_scene_types();
	
	register_editor_types();

	register_platform_apis();
	
	//register_module_types();

	g_audio = memnew(AudioServer);
	
	register_driver_types();
	
	//initialize_physics();

	register_core_singletons();
	
	register_server_singletons();
	
	OS::get_singleton()->set_cmdline(exepath, { argv, argv + argc });

	g_text = memnew(TextServer);

	g_input = memnew(Input);

	g_display = memnew(DISPLAY_SERVER_DEFAULT("ism", DS::WindowMode_Maximized, { 1280, 720 }));
	g_display->window_set_close_callback([](auto)
	{
		SceneTree::get_singleton()->get_root()->propagate_notification(Node::Notification_WM_CloseRequest);
		SceneTree::get_singleton()->quit();
	});
	g_display->window_set_focus_callback([](auto, auto focused)
	{
		SceneTree::get_singleton()->get_root()->propagate_notification(focused ? Node::Notification_WM_FocusIn : Node::Notification_WM_FocusOut);
	});
	g_display->window_set_mouse_enter_callback([](auto, auto entered)
	{
		SceneTree::get_singleton()->get_root()->propagate_notification(entered ? Node::Notification_WM_MouseEnter : Node::Notification_WM_MouseExit);
	});
	g_display->window_set_char_callback([](auto, auto c)
	{
		Input * input{ Input::get_singleton() };
		input->m_typed = c;
	});
	g_display->window_set_key_callback([](auto, auto key, auto, auto action, auto)
	{
		Input * input{ Input::get_singleton() };
		input->m_keys_down[key] = action != Input::RELEASE;
		input->m_is_shift = input->m_keys_down[Input::Key_LeftShift] || input->m_keys_down[Input::Key_RightShift];
		input->m_is_ctrl = input->m_keys_down[Input::Key_LeftCtrl] || input->m_keys_down[Input::Key_RightCtrl];
		input->m_is_alt = input->m_keys_down[Input::Key_LeftAlt] || input->m_keys_down[Input::Key_RightAlt];
		input->m_is_super = input->m_keys_down[Input::Key_LeftSuper] || input->m_keys_down[Input::Key_RightSuper];
	});
	g_display->window_set_mouse_button_callback([](auto, auto button, auto action, auto)
	{
		Input * input{ Input::get_singleton() };
		input->m_mouse_down[button] = action != Input::RELEASE;
	});
	g_display->window_set_mouse_position_callback([](auto, auto x, auto y)
	{
		Input * input{ Input::get_singleton() };
		input->m_mouse_position = { (float_t)x, (float_t)y };
	});
	g_display->window_set_scroll_callback([](auto, auto x, auto y)
	{
		Input * input{ Input::get_singleton() };
		input->m_scroll = { (float_t)x, (float_t)y };
	});

	g_renderer = memnew(RenderingServerDefault);

	g_imgui = VALIDATE(ImGui::CreateContext());
	g_imgui->IO.LogFilename = nullptr;
	g_imgui->IO.IniFilename = nullptr;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ASSERT(ImGui_Init());

	return Error_None;
}

bool Main::start()
{
	Path script{};

	Ref<MainLoop> main_loop{};

	if (editor) { main_loop = memnew(SceneTree); }

	TYPE main_loop_type{};

	if (script) { /* TODO: load main loop from script */ }

	if (!main_loop && !main_loop_type) {
		main_loop_type = typeof<SceneTree>();
	}

	if (!main_loop) {
		ASSERT(TYPE::check_(main_loop_type));
		main_loop = main_loop_type();
		ASSERT(main_loop);
	}

	if (isinstance<SceneTree>(main_loop))
	{
		SceneTree * tree{ (SceneTree *)*main_loop };

		Window * root{ tree->get_root() };

#if TOOLS_ENABLED
		if (editor) { root->add_child<EditorNode>(); }
#endif

		// etc...
	}
	
	OS::get_singleton()->set_main_loop(main_loop);

	main_loop->initialize();

	return true;
}

bool Main::iteration()
{
	++g_iterating; ON_SCOPE_EXIT(&) { --g_iterating; };
	
	Clock const loop_timer{};
	static Duration delta_time{ 16_ms };
	ON_SCOPE_EXIT(&) { delta_time = loop_timer.get_elapsed_time(); };

	// TODO: physics stuff goes here

	bool should_close{ false };

	g_display->poll_events();

	Input * input{ Input::get_singleton() };
	input->m_mouse_delta = input->m_mouse_position - input->m_prev_mouse_position;
	input->m_prev_mouse_position = input->m_mouse_position;
	for (int32_t i = 0; i < Input::MouseButton_MAX; ++i) {

		input->m_mouse_down_duration[i] = input->m_mouse_down[i]
			? (input->m_mouse_down_duration[i] < 0.f
				? 0.f
				: input->m_mouse_down_duration[i] + delta_time)
			: -1.f;
	}
	for (int32_t i = 0; i < Input::Key_MAX; ++i) {
		input->m_keys_down_duration[i] = input->m_keys_down[i]
			? (input->m_keys_down_duration[i] < 0.f
				? 0.f
				: input->m_keys_down_duration[i] + delta_time)
			: -1.f;
	}
	ON_SCOPE_EXIT(&) {
		input->m_scroll = { 0.f, 0.f };
		input->m_typed = '0';
	};

	ImGui_NewFrame();
	if (OS::get_singleton()->get_main_loop()->process(delta_time)) { should_close = true; }
	ImGui::Render();

	RD::get_singleton()->draw_list_begin_for_screen();
	ImGui_RenderDrawData(&g_imgui->Viewports[0]->DrawDataP);
	RD::get_singleton()->draw_list_end();

	if (g_imgui->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	g_display->swap_buffers();

	return should_close;
}

void Main::cleanup()
{
	//ResourceLoader::remove_custom_loaders();
	//ResourceSaver::remove_custom_savers();

	OS::get_singleton()->get_main_loop()->finalize();
	OS::get_singleton()->delete_main_loop();

	//ScriptServer::finish_languages();

#if TOOLS_ENABLED
	unregister_editor_types();
#endif

	unregister_driver_types();
	//unregister_module_types();
	unregister_platform_apis();
	unregister_server_types();
	unregister_scene_types();

	memdelete(g_audio);

	OS::get_singleton()->finalize();

	ImGui_Shutdown();
	ImGui::DestroyContext(g_imgui);

	g_renderer->finalize();
	memdelete(g_renderer);
	memdelete(g_display);
	
	memdelete(g_input);
	memdelete(g_text);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(g_internals);
	OS::get_singleton()->finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */