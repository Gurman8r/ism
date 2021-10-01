/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>

#ifdef TOOLS_ENABLED
#include <editor/editor_node.hpp>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef TOOLS_ENABLED
#include <editor/register_editor_types.hpp>
#endif

#ifdef OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

#ifdef SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define DISPLAY_SERVER_DEFAULT DisplayServerWindows
#endif

#include <servers/rendering/rendering_server_default.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(Main::g_frame_count) {};
MEMBER_IMPL(Main::g_frame_index) {};
MEMBER_IMPL(Main::g_iterating) {};

static Internals *			g_internals{};
static Input *				g_input{};
static DisplayServer *		g_display_server{};
static RenderingServer *	g_rendering_server{};

static bool editor{ true };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error Main::setup(cstring exepath, int32_t argc, char * argv[])
{
	get_os().initialize();
	
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
	
	get_os().set_cmdline(exepath, { argv, argv + argc });

	g_input = memnew(Input);

	g_display_server = memnew(DISPLAY_SERVER_DEFAULT({
		"ism",
		{ { 1280, 720 }, { 8, 8, 8, 8 }, -1 },
		{ RendererAPI_OpenGL, 4, 6, RendererProfile_Compat, 24, 8, true, false },
		WindowHints_Default_Maximized & ~(WindowHints_Doublebuffer)
		}));

	g_rendering_server = memnew(RenderingServerDefault());

	return Error_None;
}

bool Main::start()
{
	String script{};

	Ref<MainLoop> main_loop{};

	if (editor) { main_loop = memnew(SceneTree); }

	TYPE main_loop_type{};

	if (script != "") {}

	if (!main_loop && !main_loop_type) { main_loop_type = typeof<SceneTree>(); }

	if (!main_loop)
	{
		VERIFY(TYPE::check_(main_loop_type));

		main_loop = call_object(main_loop_type);
	}

	if (isinstance<SceneTree>(main_loop))
	{
		Ref<SceneTree> tree{ main_loop };

#ifdef TOOLS_ENABLED
		if (editor) { tree->get_root()->add_child(memnew(EditorNode)); }
#endif // TOOLS_ENABLED
	}

	get_os().set_main_loop(main_loop);

	return true;
}

bool Main::iteration()
{
	++g_iterating;

	bool should_exit{ false };

	if (get_os().get_main_loop()->process(16_ms)) {
		should_exit = true;
	}

	++g_frame_count;

	--g_iterating;

	return should_exit;
}

void Main::cleanup()
{
	//ResourceLoader::remove_custom_loaders();
	//ResourceSaver::remove_custom_savers();

	get_os().delete_main_loop();

	//ScriptServer::finish_languages();

#ifdef TOOLS_ENABLED
	unregister_editor_types();
#endif

	unregister_driver_types();
	//unregister_module_types();
	unregister_platform_apis();
	unregister_server_types();
	unregister_scene_types();

	//memdelete(g_audio_server);
	//memdelete(g_camera_server);

	get_os().finalize();

	g_rendering_server->finalize();
	memdelete(g_rendering_server);
	memdelete(g_display_server);
	
	memdelete(g_input);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(g_internals);
	get_os().finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */