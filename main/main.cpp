/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

#if TOOLS_ENABLED
#include <editor/register_editor_types.hpp>
#endif

#include <servers/camera_server.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering_server.hpp>

#ifdef ISM_OS_WINDOWS
#include <platform/windows/windows_display_server.hpp>
#define DISPLAY_SERVER_DEFAULT Windows_DisplayServer
#endif

#ifdef ISM_RENDERER_OPENGL
#include <drivers/opengl/opengl_rendering_device.hpp>
#define RENDERING_DEVICE_DEFAULT OpenGL_RenderingDevice
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(Main::g_frame_count) {};
MEMBER_IMPL(Main::g_frame_index) {};
MEMBER_IMPL(Main::g_iterating) {};

static Input * g_input{};
static Internals * g_internals{};
static CameraServer * g_camera_server{};
static DisplayServer * g_display_server{};
static RenderingServer * g_rendering_server{};

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

	register_core_singletons();

	register_server_types();
	
	register_scene_types();

#if TOOLS_ENABLED
	register_editor_types();
#endif

	register_platform_apis();

	//register_module_types();
	
	register_driver_types();

	//initialize_physics();
	
	//register_server_singletons();
	
	get_os().set_cmdline(exepath, { argv, argv + argc });

	return Error_None;
}

bool Main::start()
{
	g_display_server = memnew(DISPLAY_SERVER_DEFAULT);

	get_os().set_main_loop(memnew(SceneTree));

	//ResourceLoader::add_custom_loaders();
	
	//ResourceSaver::add_custom_savers();

	return true;
}

bool Main::iteration()
{
	++g_iterating;

	bool should_exit{ false };

	++g_frame_count;

	--g_iterating;

	return should_exit;
}

void Main::cleanup()
{
	//ResourceLoader::remove_custom_loaders();
	
	//ResourceSaver::remove_custom_savers();

	get_os().delete_main_loop();

	memdelete(g_display_server);

	//ScriptServer::finish_languages();

#ifdef TOOLS_ENABLED
	unregister_editor_types();
#endif

	unregister_driver_types();
	
	//unregister_module_types();
	
	unregister_platform_apis();
	
	unregister_scene_types();
	
	unregister_server_types();

	get_os().finalize();

	unregister_core_driver_types();
	
	unregister_core_types();

	memdelete(g_internals);

	get_os().finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */