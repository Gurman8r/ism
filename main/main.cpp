/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/main/imgui.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <modules/register_module_types.hpp>
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
#include <servers/physics_server.hpp>
#include <servers/text_server.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

i32 Main::m_iterating{};
u64 Main::m_frame{};

static Internals *			internals{};
static Performance *		performance{};
static Input *				input{};
static AudioServer *		audio_server{};
static DisplayServer *		display_server{};
static RenderingServer *	rendering_server{};
static PhysicsServer *		physics_server{};
static TextServer *			text_server{};
static ImGuiContext *		gui_context{};

static bool editor{ true };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ Main::setup(cstring exepath, i32 argc, char * argv[])
{
	OS::get_singleton()->initialize();

	internals = memnew(Internals);
	performance = memnew(Performance);

	register_core_types();
	register_core_driver_types();
	register_core_settings();
	OS::get_singleton()->set_cmdline(exepath, { argv, argv + argc });
	register_server_types();

	input = memnew(Input);
	text_server = memnew(TextServer);
	display_server = memnew(DISPLAY_SERVER_DEFAULT("ism", DS::WindowMode_Maximized, { 1280, 720 }));
	rendering_server = memnew(RenderingServerDefault);
	audio_server = memnew(AudioServer);

	register_core_singletons();
	register_scene_types();
	register_driver_types();
#if TOOLS_ENABLED
	register_editor_types();
#endif
	register_platform_apis();

	//initialize_theme();
	
	physics_server = memnew(PhysicsServer);
	
	register_server_singletons();
	
	//init_languages();
	
	gui_context = VALIDATE(ImGui::CreateContext());
	gui_context->IO.LogFilename = nullptr;
	gui_context->IO.IniFilename = nullptr;
	gui_context->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	gui_context->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	gui_context->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ASSERT(ImGui_Init());

	return Error_None;
}

bool Main::start()
{
	Path script{};

	Ref<MainLoop> main_loop{};

	if (editor) { main_loop = memnew(SceneTree); }

	TYPE main_loop_type{};

	if (script) {
		/* TODO: load main loop from script */
	}

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
	++m_iterating; ON_SCOPE_EXIT(&) { --m_iterating; };

	Clock const loop_timer{};
	static Duration delta_time{ 16_ms };
	ON_SCOPE_EXIT(&) { delta_time = loop_timer.get_elapsed_time(); };

	++m_frame;

	bool should_close{ false };

	// TODO: physics_server stuff goes here

	display_server->poll_events();

	Input::get_singleton()->iteration(delta_time);

	ImGui_NewFrame();
	if (OS::get_singleton()->get_main_loop()->process(delta_time)) { should_close = true; }
	ImGui::Render();

	RD::get_singleton()->draw_list_begin_for_screen();
	ImGui_RenderDrawData(&gui_context->Viewports[0]->DrawDataP);
	RD::get_singleton()->draw_list_end();

	if (gui_context->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	display_server->swap_buffers();

	return should_close;
}

void Main::cleanup()
{
	//remove_custom_loaders();
	//remove_custom_savers();

	OS::get_singleton()->get_main_loop()->finalize();
	OS::get_singleton()->delete_main_loop();

	//finish_languages();

#if TOOLS_ENABLED
	unregister_editor_types();
#endif
	unregister_driver_types();
	unregister_platform_apis();
	unregister_server_types();
	unregister_scene_types();

	memdelete(audio_server);

	OS::get_singleton()->finalize();

	ImGui_Shutdown();
	ImGui::DestroyContext(gui_context);
	rendering_server->finalize();

	memdelete(rendering_server);
	memdelete(display_server);
	memdelete(physics_server);
	memdelete(text_server);
	memdelete(input);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(performance);
	memdelete(internals);

	OS::get_singleton()->finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */