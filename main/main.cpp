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

MEMBER_IMPL(Main::g_iterating) {};

static Internals *			g_internals{};
static EventBus *			g_bus{};
static Input *				g_input{};
static AudioServer *		g_audio{};
static DisplayServer *		g_display{};
static RenderingServer *	g_renderer{};
static ImGuiContext *		g_imgui{};
static TextServer *			g_text{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ Main::setup(cstring exepath, int32_t argc, char * argv[])
{
	SYSTEM->initialize();
	
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
	
	SYSTEM->set_cmdline(exepath, { argv, argv + argc });

	// event system
	g_bus = memnew(EventBus);

	// text server
	g_text = memnew(TextServer);

	// input server
	g_input = memnew(Input);

	// display server
	g_display = memnew(DISPLAY_SERVER_DEFAULT("ism", DS::WindowMode_Maximized, { 1280, 720 }));
	DS::WindowID main_window{ g_display->get_current_context() };
	g_display->window_set_char_callback(main_window, [](auto, auto c) { g_input->m_last_char = (char)c; });
	g_display->window_set_mouse_button_callback(main_window, [](auto, auto b, auto a, auto) { g_input->m_mouse_down.write(b, a != InputAction_Release); });
	g_display->window_set_mouse_position_callback(main_window, [](auto, auto x, auto y) { g_input->m_mouse_pos = { (float_t)x, (float_t)y }; });
	g_display->window_set_scroll_callback(main_window, [](auto, auto x, auto y) { g_input->m_scroll = { (float_t)x, (float_t)y }; });
	g_display->window_set_key_callback(main_window, [](auto, auto k, auto, auto a, auto) {
		g_input->m_keys_down.write(k, a != InputAction_Release);
		g_input->m_is_shift = g_input->m_keys_down[KeyCode_LeftShift] || g_input->m_keys_down[KeyCode_RightShift];
		g_input->m_is_ctrl = g_input->m_keys_down[KeyCode_LeftCtrl] || g_input->m_keys_down[KeyCode_RightCtrl];
		g_input->m_is_alt = g_input->m_keys_down[KeyCode_LeftAlt] || g_input->m_keys_down[KeyCode_RightAlt];
		g_input->m_is_super = g_input->m_keys_down[KeyCode_LeftSuper] || g_input->m_keys_down[KeyCode_RightSuper];
	});

	// rendering server
	g_renderer = memnew(RenderingServerDefault());

	// imgui
	g_imgui = VALIDATE(ImGui::CreateContext());
	g_imgui->IO.LogFilename = nullptr;
	g_imgui->IO.IniFilename = nullptr;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	g_imgui->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ASSERT(ImGui_Init(main_window));

	return Error_None;
}

bool Main::start()
{
	Path script{};

	Ref<MainLoop> main_loop{};

	if (editor) { main_loop = memnew(SceneTree); }

	TYPE main_loop_type{};

	if (script) { /* TODO: load main loop from script */ }

	if (!main_loop && !main_loop_type) { main_loop_type = typeof<SceneTree>(); }

	if (!main_loop)
	{
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
	}
	
	SYSTEM->set_main_loop(main_loop);

	main_loop->initialize();

	return true;
}

bool Main::iteration()
{
	++g_iterating; ON_SCOPE_EXIT(&) { --g_iterating; };
	
	// iteration timer
	Clock const loop_timer{};
	static Duration delta_time{ 16_ms };
	ON_SCOPE_EXIT(&) { delta_time = loop_timer.get_elapsed_time(); };

	// TODO: physics stuff goes here

	bool should_close{ false };

	// poll events
	g_display->poll_events();

	// update input
	static Vec2 last_mouse_pos{};
	g_input->m_mouse_delta = g_input->m_mouse_pos - last_mouse_pos;
	last_mouse_pos = g_input->m_mouse_pos;
	for (size_t i = 0; i < MouseButton_MAX; ++i) {
		g_input->m_mouse_down_duration[i] = (g_input->m_mouse_down[i]
			? (g_input->m_mouse_down_duration[i] < 0.f
				? 0.f
				: g_input->m_mouse_down_duration[i] + delta_time)
			: -1.f);
	}
	for (size_t i = 0; i < KeyCode_MAX; ++i) {
		g_input->m_keys_down_duration[i] = (g_input->m_keys_down[i]
			? (g_input->m_keys_down_duration[i] < 0.f
				? 0.f
				: g_input->m_keys_down_duration[i] + delta_time)
			: -1.f);
	}
	ON_SCOPE_EXIT(&) {
		g_input->m_scroll = {};
		g_input->m_last_char = 0;
	};

	// update main loop
	ImGui_NewFrame();
	if (SYSTEM->get_main_loop()->process(delta_time)) { should_close = true; }
	ImGui::Render();

	// render
	RENDERING_DEVICE->draw_list_begin_for_screen(g_display->get_current_context());
	ImGui_RenderDrawData(&g_imgui->Viewports[0]->DrawDataP);
	RENDERING_DEVICE->draw_list_end();

	if (g_imgui->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		DS::WindowID backup_context{ g_display->get_current_context() };
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		g_display->set_current_context(backup_context);
	}

	return should_close;
}

void Main::cleanup()
{
	//ResourceLoader::remove_custom_loaders();
	//ResourceSaver::remove_custom_savers();

	SYSTEM->get_main_loop()->finalize();
	SYSTEM->delete_main_loop();

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

	SYSTEM->finalize();

	ImGui_Shutdown();
	ImGui::DestroyContext(g_imgui);

	g_renderer->finalize();
	memdelete(g_renderer);
	memdelete(g_display);
	
	memdelete(g_input);
	memdelete(g_text);
	memdelete(g_bus);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(g_internals);
	SYSTEM->finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */