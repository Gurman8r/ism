#include <main/main.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/object/script.hpp>
#include <core/io/file_access_zip.hpp>

#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

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
	static Packages *			packages{};
	static ZipArchive *			zip_archive{};
	static ProjectSettings *	project{};
	static Performance *		performance{};
	static ExtensionManager *	extensions{};
	static ScriptServer *		scripts{};
	static Input *				input{};
	static ResourceLoader *		loader{};
	static ResourceSaver *		saver{};

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
		Error_ error{ Error_OK };

		SYSTEM->initialize();

		internals = memnew(Internals);
		engine = memnew(Engine);

		if (!(packages = Packages::get_singleton())) { packages = memnew(Packages); }
		if (!(zip_archive = ZipArchive::get_singleton())) { zip_archive = memnew(ZipArchive); }
		packages->add_pack_source(zip_archive);

		packages->add_pack("../assets/test.zip", true, 0);
		if (auto f{ FileAccess::open("test.zip://data/sub.txt", FileMode_Read) }) {
			while (String line{ f->read_line() }) {
				PRINT_LINE(line);
			}
		}

		performance = memnew(Performance);
		project = memnew(ProjectSettings);
		extensions = memnew(ExtensionManager);
		scripts = memnew(ScriptServer);
		input = memnew(Input);
		loader = memnew(ResourceLoader);
		saver = memnew(ResourceSaver);

		register_core_types();
		register_core_driver_types();

		project->setup(exepath);
		register_core_settings();
		initialize_modules(ExtensionInitializationLevel_Core);
		register_core_extensions();
		register_core_singletons();
		SYSTEM->set_cmdline(exepath, { argv, argv + argc });

		text = TS::create();
		display = DS::create("ism", DS::WindowMode_Maximized, { 0, 0 }, { 1280, 720 }, 0, error);
		graphics = RS::create();
		audio = AS::create();
		physics = PS::create();

		register_server_types();
		initialize_modules(ExtensionInitializationLevel_Servers);
		extensions->initialize_extensions(ExtensionInitializationLevel_Servers);
		register_server_singletons();

		register_scene_types();
		register_driver_types();
		initialize_modules(ExtensionInitializationLevel_Scene);
		extensions->initialize_extensions(ExtensionInitializationLevel_Scene);
		register_scene_singletons();

#if TOOLS_ENABLED
		register_editor_types();
		initialize_modules(ExtensionInitializationLevel_Editor);
		extensions->initialize_extensions(ExtensionInitializationLevel_Editor);
		register_editor_singletons();
#endif

		register_platform_apis();

		//initialize_theme();

		//initialize_physics();
	
		scripts->initialize_languages();
	
		imgui_context = ImGui_Initialize();

		return error;
	}

	bool Main::start()
	{
		String script{};

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
	
		SYSTEM->set_main_loop(main_loop);

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

		INPUT->iteration(delta_time);

		ImGui_BeginFrame(imgui_context);

		if (SYSTEM->get_main_loop()->process(delta_time)) { should_close = true; }

		ImGui::Render();
		RENDERING_DEVICE->draw_list_begin_for_screen();
		ImGui_RenderDrawData(&imgui_context->Viewports[0]->DrawDataP);
		RENDERING_DEVICE->draw_list_end();

		ImGui_EndFrame(imgui_context);
	
		return should_close;
	}

	void Main::cleanup()
	{
		//remove_custom_loaders();
		//remove_custom_savers();

		SYSTEM->delete_main_loop();

		scripts->finalize_languages();

#if TOOLS_ENABLED
		finalize_modules(ExtensionInitializationLevel_Editor);
		extensions->finalize_extensions(ExtensionInitializationLevel_Editor);
		unregister_editor_types();
#endif

		finalize_modules(ExtensionInitializationLevel_Scene);
		extensions->finalize_extensions(ExtensionInitializationLevel_Scene);
		unregister_platform_apis();
		unregister_driver_types();
		unregister_scene_types();

		//finalize_theme();

		finalize_modules(ExtensionInitializationLevel_Servers);
		extensions->finalize_extensions(ExtensionInitializationLevel_Servers);
		unregister_server_types();

		SYSTEM->finalize();

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

		memdelete(loader);
		memdelete(saver);
		memdelete(input);
		memdelete(scripts);
		memdelete(extensions);
		memdelete(project);
		memdelete(performance);
		memdelete(packages);
		memdelete(engine);
		memdelete(internals);
		SYSTEM->finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}