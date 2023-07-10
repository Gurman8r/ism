#include <main/main.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>

#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

#include <core/extension/extension_manager.hpp>
#include <core/object/script.hpp>
#include <core/io/zip.hpp>

#include <servers/audio_server.hpp>
#include <servers/physics_server.hpp>
#include <servers/rendering_server.hpp>
#include <servers/text_server.hpp>
#include <scene/main/scene_tree.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#include <editor/register_editor_types.hpp>
#endif

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

#include <cxxopts.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Main::m_iterating{};
	u64 Main::m_frame{};

	static Engine *				_engine{};
	static Performance *		_perf{};
	static Input *				_input{};
	static ProjectSettings *	_globals{};

	static PackedData *			_packed_data{};
	static ZipArchive *			_zip_archive{};

	static AudioServer *		_audio_server{};
	static DisplayServer *		_display_server{};
	static RenderingServer *	_rendering_server{};
	static PhysicsServer *		_physics_server{};
	static TextServer *			_text_server{};

	static cxxopts::ParseResult options;
	static bool cmdline{};
	static bool editor{ true };

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

		os()->initialize();
		_engine = memnew(Engine);
		register_core_types();
		register_core_driver_types();

		_perf = memnew(Performance);
		_input = memnew(Input);
		_globals = memnew(ProjectSettings);
		register_core_settings();

		if (!(_packed_data = PackedData::get_singleton())) { _packed_data = memnew(PackedData); }
		if (!(_zip_archive = ZipArchive::get_singleton())) { _zip_archive = memnew(ZipArchive); }
		_packed_data->add_package_source(_zip_archive);

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

		_globals->setup(exec_path);
		register_core_extensions();
		os()->set_cmdline(exec_path, args);

		// display server
		_display_server = DS::create(os()->get_exec_path().stem(), DS::WindowMode_Maximized, { 0, 0 }, { 1280, 720 }, 0, error); 
		if (error != Error_OK) {
			CRASH("failed creating _display_server server");
		}
		_display_server->set_native_icon("res://icons/" + os()->get_exec_path().stem() + ".png");
		
		// rendering server
		_rendering_server = RS::create();
		_rendering_server->initialize();
		
		// text server
		_text_server = memnew(TS);
		
		// physics server
		_physics_server = memnew(PS);
		
		// audio server
		_audio_server = memnew(AS);

		register_core_singletons();

		register_server_types();
		extension_manager()->initialize_extensions(ExtensionInitializationLevel_Servers);
		register_server_singletons();

		register_scene_types();
		register_driver_types();
		extension_manager()->initialize_extensions(ExtensionInitializationLevel_Scene);
		register_scene_singletons();

#if TOOLS_ENABLED
		register_editor_types();
		extension_manager()->initialize_extensions(ExtensionInitializationLevel_Editor);
		register_editor_singletons();
#endif

		//initialize_theme();

		//initialize_physics();
	
		script_server()->initialize_languages();
	
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
	
		os()->set_main_loop(main_loop);

		return true;
	}

	bool Main::iteration()
	{
		++m_iterating; ON_SCOPE_EXIT(&) { --m_iterating; };

		Clock const loop_timer{};
		static Duration dt{ 16_ms };
		ON_SCOPE_EXIT(&) { dt = loop_timer.get_time(); };

		++m_frame;

		bool should_close{ false };

		// TODO: _physics_server stuff goes here

		_input->iteration(dt);

		if (os()->get_main_loop()->process(dt)) { should_close = true; }
	
		return should_close;
	}

	void Main::cleanup(bool force)
	{
		//remove_custom_loaders();
		//remove_custom_savers();

		os()->delete_main_loop();

		script_server()->finalize_languages();

#if TOOLS_ENABLED
		extension_manager()->finalize_extensions(ExtensionInitializationLevel_Editor);
		unregister_editor_types();
#endif

		extension_manager()->finalize_extensions(ExtensionInitializationLevel_Scene);
		unregister_driver_types();
		unregister_scene_types();

		//finalize_theme();

		extension_manager()->finalize_extensions(ExtensionInitializationLevel_Servers);
		unregister_server_types();

		memdelete(_audio_server);
		os()->finalize();
		_rendering_server->finalize();
		memdelete(_rendering_server);
		memdelete(_display_server);
		memdelete(_physics_server);
		memdelete(_text_server);

		memdelete(_packed_data);
		memdelete(_input);
		memdelete(_globals);
		memdelete(_perf);

		unregister_core_driver_types();
		unregister_core_extensions();
		unregister_core_types();
		memdelete(_engine);
		os()->finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}