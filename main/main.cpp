#include <main/main.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/object/script.hpp>

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <modules/register_module_types.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

#include <servers/audio_server.hpp>
#include <servers/physics_server.hpp>
#include <servers/rendering_server.hpp>
#include <servers/text_server.hpp>

#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#include <editor/register_editor_types.hpp>
#endif

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Main::m_iterating{};
	u64 Main::m_frame{};

	static Internals *			internals{};
	static Engine *				engine{};
	static ProjectSettings *	project{};
	static Performance *		perf{};
	static ExtensionManager *	ext{};
	static ScriptServer *		scr{};
	static Input *				input{};

	static AudioServer *		audio{};
	static DisplayServer *		display{};
	static RenderingServer *	graphics{};
	static PhysicsServer *		physics{};
	static TextServer *			text{};

	static bool editor{ true };

	static ImGuiContext * imgui_context{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ Main::setup(cstring exepath, i32 argc, char * argv[])
	{
		Error_ error{};

		OS::get_singleton()->initialize();

		internals = memnew(Internals);
		engine = memnew(Engine);
		register_core_types();
		register_core_driver_types();
		project = memnew(ProjectSettings);
		perf = memnew(Performance);
		ext = memnew(ExtensionManager);
		scr = memnew(ScriptServer);
		input = memnew(Input);
		register_core_settings();
		initialize_modules(ExtensionInitializationLevel_Core);
		register_core_extensions();
		register_core_singletons();

		OS::get_singleton()->set_cmdline(exepath, { argv, argv + argc });

		text = TS::create();
		display = DS::create("ism", DS::WindowMode_Maximized, { 0, 0 }, { 1280, 720 }, 0, error);
		graphics = RS::create();
		audio = AS::create();
		physics = PS::create();

		register_server_types();
		initialize_modules(ExtensionInitializationLevel_Servers);
		ext->initialize_extensions(ExtensionInitializationLevel_Servers);
		register_server_singletons();

		register_scene_types();
		register_driver_types();
		initialize_modules(ExtensionInitializationLevel_Scene);
		ext->initialize_extensions(ExtensionInitializationLevel_Scene);

#if TOOLS_ENABLED
		register_editor_types();
		initialize_modules(ExtensionInitializationLevel_Editor);
		ext->initialize_extensions(ExtensionInitializationLevel_Editor);
#endif

		register_platform_apis();

		//initialize_theme();

		register_scene_singletons();

		//initialize_physics();
	
		scr->initialize_languages();
	
		imgui_context = ImGui_Initialize();

		return error;
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

		// TODO: physics stuff goes here

		Input::get_singleton()->iteration(delta_time);

		ImGui_BeginFrame(imgui_context);

		if (OS::get_singleton()->get_main_loop()->process(delta_time)) { should_close = true; }

		ImGui::Render();
		RD::get_singleton()->draw_list_begin_for_screen();
		ImGui_RenderDrawData(&imgui_context->Viewports[0]->DrawDataP);
		RD::get_singleton()->draw_list_end();

		ImGui_EndFrame(imgui_context);
	
		return should_close;
	}

	void Main::cleanup()
	{
		//remove_custom_loaders();
		//remove_custom_savers();

		OS::get_singleton()->delete_main_loop();

		scr->finalize_languages();

#if TOOLS_ENABLED
		finalize_modules(ExtensionInitializationLevel_Editor);
		ext->finalize_extensions(ExtensionInitializationLevel_Editor);
		unregister_editor_types();
#endif

		finalize_modules(ExtensionInitializationLevel_Scene);
		ext->finalize_extensions(ExtensionInitializationLevel_Scene);
		unregister_platform_apis();
		unregister_driver_types();
		unregister_scene_types();

		//finalize_theme();

		finalize_modules(ExtensionInitializationLevel_Servers);
		ext->finalize_extensions(ExtensionInitializationLevel_Servers);
		unregister_server_types();

		OS::get_singleton()->finalize();

		ImGui_Finalize(imgui_context);

		graphics->finalize();

		memdelete(audio);
		memdelete(graphics);
		memdelete(display);
		memdelete(physics);
		memdelete(text);

		unregister_core_driver_types();
		unregister_core_extensions();
		unregister_core_types();

		memdelete(input);
		memdelete(scr);
		memdelete(ext);
		memdelete(perf);
		memdelete(project);
		memdelete(engine);
		memdelete(internals);

		OS::get_singleton()->finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}