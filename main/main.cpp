#include <main/main.hpp>
#include <cxxopts.hpp>

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>
#include <core/io/zip.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/object/script.hpp>

#include <servers/audio_server.hpp>
#include <servers/physics_server.hpp>
#include <servers/rendering_server.hpp>
#include <servers/text_server.hpp>
#include <scene/main/scene_tree.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#include <editor/register_editor_types.hpp>
#endif

#include <scene/gui/imgui.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Main::m_iterating{};
	u64 Main::m_frame{};

	static Engine *				engine{};
	static Performance *		performance{};
	static Input *				input{};
	static ProjectSettings *	settings{};

	static PackedData *			packed_data{};
	static ZipArchive *			zip_archive{};

	static AudioServer *		audio{};
	static DisplayServer *		display{};
	static RenderingServer *	graphics{};
	static PhysicsServer *		physics{};
	static TextServer *			text{};

	static cxxopts::ParseResult options;
	static bool cmdline{};
	static bool editor{ true };

	static ImGuiContext * imgui{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Main::print_help()
	{
	}

	bool Main::is_cmdline_tool()
	{
		return cmdline;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ Main::setup(cstring exec_path, i32 argc, char * argv[])
	{
		PRINT_LINE(exec_path);

		Error_ error{};

		get_os()->initialize();
		engine = memnew(Engine);
		register_core_types();
		register_core_driver_types();

		performance = memnew(Performance);
		input = memnew(Input);
		settings = memnew(ProjectSettings);
		register_core_settings();

		if (!(packed_data = PackedData::get_singleton())) { packed_data = memnew(PackedData); }
		if (!(zip_archive = ZipArchive::get_singleton())) { zip_archive = memnew(ZipArchive); }
		packed_data->add_package_source(zip_archive);

		Vector<String> args{ argv, argv + argc };
		cxxopts::Options options_parser{ exec_path, VERSION_FULL_NAME };
		options_parser.add_options()
			("e,editor", "enable editor", cxxopts::value<bool>()->default_value("false"))
			("c,cmdline", "enable command line", cxxopts::value<bool>()->default_value("false"))
			("s,script", "main script", cxxopts::value<std::string>())
			;

		options = options_parser.parse(argc, argv);
		if (options.count("c")) { cmdline = true; }
		if (options.count("e")) { editor = true; }

		settings->setup(exec_path);
		register_core_extensions();
		get_os()->set_cmdline(exec_path, args);

		// display server
		display = DS::create(get_os()->get_exec_path().stem(), DS::WindowMode_Maximized, { 0, 0 }, { 1280, 720 }, 0, error); 
		if (error != Error_OK) {
			CRASH("failed creating display server");
		}
		display->set_native_icon("res://icons/" + get_os()->get_exec_path().stem() + ".png");
		
		// rendering server
		graphics = RS::create();
		
		// text server
		text = memnew(TS);
		
		// physics server
		physics = memnew(PS);
		
		// audio server
		audio = memnew(AS);

		register_core_singletons();

		register_server_types();
		get_ext()->initialize_extensions(ExtensionInitializationLevel_Servers);
		register_server_singletons();

		register_scene_types();
		register_driver_types();
		get_ext()->initialize_extensions(ExtensionInitializationLevel_Scene);
		register_scene_singletons();

#if TOOLS_ENABLED
		register_editor_types();
		get_ext()->initialize_extensions(ExtensionInitializationLevel_Editor);
		register_editor_singletons();
#endif

		//initialize_theme();

		//initialize_physics();
	
		get_scr()->initialize_languages();
	
		imgui = VALIDATE(ImGui_Initialize());

		return Error_OK;
	}

	bool Main::start()
	{
		String script{};

		Ref<MainLoop> main_loop{};

#if TOOLS_ENABLED
		if (editor) { main_loop = memnew(SceneTree); }
#endif

		TypeRef main_loop_type{};

		if (script) {
			// TODO: load main loop from script
		}

		if (!main_loop && !main_loop_type) {
			main_loop_type = typeof<SceneTree>();
		}

		if (!main_loop) {
			ASSERT(TypeRef::check_(main_loop_type));
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
	
		get_os()->set_main_loop(main_loop);

		return true;
	}

	bool Main::iteration()
	{
		++m_iterating; ON_SCOPE_EXIT(&) { --m_iterating; };

		Clock const loop_timer{};
		static Duration delta_time{ 16_ms };
		ON_SCOPE_EXIT(&) { delta_time = loop_timer.get_time(); };

		++m_frame;

		bool should_close{ false };

		// TODO: physics stuff goes here

		input->iteration(delta_time);

		ImGui_BeginFrame(imgui);
		if (get_os()->get_main_loop()->process(delta_time)) { should_close = true; }
		ImGui::Render();
		get_gpu()->draw_list_begin_for_screen();
		ImGui_RenderDrawData(&imgui->Viewports[0]->DrawDataP);
		get_gpu()->draw_list_end();
		ImGui_EndFrame(imgui);
	
		return should_close;
	}

	void Main::cleanup(bool force)
	{
		//remove_custom_loaders();
		//remove_custom_savers();

		ImGui_Finalize(imgui);

		get_os()->delete_main_loop();

		get_scr()->finalize_languages();

#if TOOLS_ENABLED
		get_ext()->finalize_extensions(ExtensionInitializationLevel_Editor);
		unregister_editor_types();
#endif

		get_ext()->finalize_extensions(ExtensionInitializationLevel_Scene);
		unregister_driver_types();
		unregister_scene_types();

		//finalize_theme();

		get_ext()->finalize_extensions(ExtensionInitializationLevel_Servers);
		unregister_server_types();

		memdelete(audio);
		get_os()->finalize();
		graphics->finalize();
		memdelete(graphics);
		memdelete(display);
		memdelete(physics);
		memdelete(text);

		memdelete(packed_data);
		memdelete(input);
		memdelete(settings);
		memdelete(performance);

		unregister_core_driver_types();
		unregister_core_extensions();
		unregister_core_types();
		memdelete(engine);
		get_os()->finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}