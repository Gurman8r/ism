/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

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

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

#if SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define DISPLAY_SERVER_DEFAULT DisplayServerWindows
#endif

#include <servers/rendering/rendering_server_default.hpp>

#include <servers/text_server.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

static bool editor{ true };

MEMBER_IMPL(Main::g_iterating) {};

static Internals *			g_internals{};
static EventBus *			g_bus{};
static Input *				g_input{};
static DisplayServer *		g_display{};
static RenderingServer *	g_renderer{};
static TextServer *			g_text{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ Main::setup(cstring exepath, int32_t argc, char * argv[])
{
	SINGLETON(OS)->initialize();
	
	g_internals = memnew(Internals);

	register_core_types();
	
	register_core_driver_types();
	
	register_core_settings();

	//preregister_module_types();
	
	//preregister_server_types();
	
	register_server_types();
	
	register_scene_types();
	
#if TOOLS_ENABLED
	register_editor_types();
#endif

	register_platform_apis();
	
	//register_module_types();

	//g_camera_server = CameraServer::create();
	
	register_driver_types();
	
	//initialize_physics();

	register_core_singletons();
	
	//register_server_singletons();
	
	SINGLETON(OS)->set_cmdline(exepath, { argv, argv + argc });

	// event system
	g_bus = memnew(EventBus);

	// text server
	g_text = memnew(TextServer);

	// input server
	g_input = memnew(Input);

	// display server
	g_display = memnew(DISPLAY_SERVER_DEFAULT({
		"ism",
		{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },
		{ RendererAPI_OpenGL, 4, 6, RendererProfile_Compat, 24, 8, true, false },
		WindowHints_Default_Maximized & ~(WindowHints_Doublebuffer)
		}));
	
	WindowID main_window{ g_display->get_context_current() };
	g_display->window_set_char_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowCharEvent(x...)); });
	g_display->window_set_char_mods_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowCharModsEvent(x...)); });
	g_display->window_set_close_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowCloseEvent(x...)); });
	g_display->window_set_content_scale_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowContentScaleEvent(x...)); });
	g_display->window_set_drop_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowDropEvent(x...)); });
	g_display->window_set_focus_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowFocusEvent(x...)); });
	g_display->window_set_framebuffer_resize_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowFramebufferResizeEvent(x...)); });
	g_display->window_set_iconify_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowIconifyEvent(x...)); });
	g_display->window_set_key_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowKeyEvent(x...)); });
	g_display->window_set_maximize_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowMaximizeEvent(x...)); });
	g_display->window_set_mouse_button_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowMouseButtonEvent(x...)); });
	g_display->window_set_mouse_enter_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowMouseEnterEvent(x...)); });
	g_display->window_set_mouse_position_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowMousePositionEvent(x...)); });
	g_display->window_set_position_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowPositionEvent(x...)); });
	g_display->window_set_refresh_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowRefreshEvent(x...)); });
	g_display->window_set_scroll_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowScrollEvent(x...)); });
	g_display->window_set_size_callback(main_window, [](auto ... x) { g_bus->fire_event(WindowSizeEvent(x...)); });

	// rendering server
	g_renderer = memnew(RenderingServerDefault());

	// initialize imgui
	if (auto ctx{ ImGui::CreateContext() }) {
		ctx->IO.LogFilename = nullptr;
		ctx->IO.IniFilename = nullptr;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ASSERT(ImGui_Init(main_window));
	}

	return Error_None;
}

bool Main::start()
{
	String script{};

	Ref<MainLoop> main_loop{};

	if (editor) { main_loop = memnew(SceneTree); }

	TYPE main_loop_type{};

	if (script != "")
	{
		/* TODO */
	}

	if (!main_loop && !main_loop_type) { main_loop_type = typeof<SceneTree>(); }

	if (!main_loop)
	{
		ASSERT(TYPE::check_(main_loop_type));

		main_loop = main_loop_type();

		ASSERT(main_loop);
	}

	if (isinstance<SceneTree>(main_loop))
	{
		Ref<SceneTree> tree{ main_loop };

		Ref<Window> root{ tree->get_root() };

#if TOOLS_ENABLED
		if (editor) { root->add_child<EditorNode>(); }
#endif
	}
	
	SINGLETON(OS)->set_main_loop(main_loop);

	main_loop->initialize();

	return true;
}

bool Main::iteration()
{
	++g_iterating;

	Timer const loop_timer{ true };
	static Duration delta_time{ 16_ms };
	SCOPE_EXIT(&) { delta_time = loop_timer.elapsed(); };

	bool should_close{ false };

	// process physics here

	SINGLETON(DisplayServer)->poll_events();

	ImGui_NewFrame();

	if (SINGLETON(OS)->get_main_loop()->process(delta_time)) { should_close = true; }

	ImGui_RenderFrame();

	--g_iterating;

	return should_close;
}

void Main::cleanup()
{
	//ResourceLoader::remove_custom_loaders();
	//ResourceSaver::remove_custom_savers();

	SINGLETON(OS)->get_main_loop()->finalize();
	SINGLETON(OS)->delete_main_loop();

	//ScriptServer::finish_languages();

#if TOOLS_ENABLED
	unregister_editor_types();
#endif

	unregister_driver_types();
	//unregister_module_types();
	unregister_platform_apis();
	unregister_server_types();
	unregister_scene_types();

	//memdelete(g_audio_server);
	//memdelete(g_camera_server);

	SINGLETON(OS)->finalize();

	ImGui_Shutdown();
	ImGui::DestroyContext();

	g_renderer->finalize();
	memdelete(g_renderer);
	memdelete(g_display);
	
	memdelete(g_input);
	memdelete(g_text);
	memdelete(g_bus);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(g_internals);
	SINGLETON(OS)->finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */