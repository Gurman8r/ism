#include <platform/windows/windows_os.hpp>
#include <platform/windows/windows_display_server.hpp>
#include <drivers/windows/windows_dir.hpp>
#include <drivers/windows/windows_file.hpp>
#include <core/config/project_settings.hpp>
#include <main/main.hpp>
#include <iostream>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	WindowsOS::WindowsOS(HINSTANCE hInstance) : OS{}
	{
		m_hinstance = hInstance ? hInstance : GetModuleHandle(NULL);
	}

	WindowsOS::~WindowsOS()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void WindowsOS::initialize()
	{
		WindowsDir::initialize();
		WindowsFile::initialize();
		WindowsDisplayServer::initialize();
	}

	void WindowsOS::finalize()
	{
	}

	void WindowsOS::finalize_core()
	{
		WindowsDisplayServer::finalize();
		WindowsFile::finalize();
		WindowsDir::finalize();
	}

	void WindowsOS::run()
	{
		if (!m_main_loop) { return; }
		m_main_loop->initialize();
		while (true) {
			display_server()->poll_events();
			if (Main::iteration()) { break; }
			display_server()->swap_buffers();
		}
		m_main_loop->finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<String> WindowsOS::get_video_adapter_driver_info() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsOS::get_stdin_string(bool blocking)
	{
		if (blocking) {
			String temp;
			std::getline(std::cin, temp.native());
			return temp;
		}
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ WindowsOS::open_dynamic_library(String path, void *& instance, bool set_library_path, String * resolved_path)
	{
		if (!File::exists(path)) {
			path += ".dll";
		}

		if (!(instance = LoadLibraryA(path.c_str()))) {
			PRINT_ERROR("failed loading library");
			return Error_Failed;
		}

		//DLL_DIRECTORY_COOKIE cookie{};
		//if (set_library_path) { cookie = AddDllDirectory((LPCWSTR)(path.root_directory().widen().c_str())); }
		//instance = VALIDATE(LoadLibraryExW((LPCWSTR)(path.widen().c_str()), nullptr, set_library_path ? LOAD_LIBRARY_SEARCH_DEFAULT_DIRS : 0));
		//if (cookie) { RemoveDllDirectory(cookie); }

		if (resolved_path) { *resolved_path = path; }
		return Error_OK;
	}

	Error_ WindowsOS::close_dynamic_library(void * instance)
	{
		if (!instance) {
			return Error_Failed;
		}

		FreeLibrary((HMODULE)instance);
		return Error_OK;
	}

	Error_ WindowsOS::get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional)
	{
		if (!instance) {
			PRINT_ERROR("dynamic library instance cannot be null");
			return Error_Failed;
		}

		if (name.empty()) {
			PRINT_ERROR("dynamic library name cannot be empty");
			return Error_Failed;
		}

		symbol = GetProcAddress((HMODULE)instance, name.c_str());
		if (!symbol && !is_optional) {
			PRINT_ERROR("failed loading dynamic library symbol: " + name);
			return Error_Failed;
		}

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ WindowsOS::execute(String const & path, Vector<String> const & args, String * pipe, i32 * exitcode, bool read_stderr, Mutex * pipe_mutex)
	{
		return Error_Failed;
	}

	Error_ WindowsOS::create_process(String const & path, Vector<String> const & args, ProcessID * child_id)
	{
		return Error_Failed;
	}

	Error_ WindowsOS::kill(ProcessID const & pid)
	{
		return Error_Failed;
	}

	i32 WindowsOS::get_pid() const
	{
		return -1;
	}

	bool WindowsOS::is_process_running(ProcessID const & pid) const
	{
		return false;
	}

	String WindowsOS::get_cwd() const
	{
		return ".";
	}

	Error_ WindowsOS::set_cwd(String const & path)
	{
		return Error_OK;
	}

	Error_ WindowsOS::shell_open(String const & path)
	{
		return Error_Failed;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsOS::get_env(String const & key) const
	{
		WCHAR env[0x7fff]; // MSDN says 32767 char is the maximum
		return (GetEnvironmentVariableW(key.widen().c_str(), env, 0x7fff) > 0)
			? Unicode(env).narrow()
			: String{};
	}

	bool WindowsOS::has_env(String const & key) const
	{
		WCHAR * env; size_t len;
		_wdupenv_s(&env, &len, key.widen().c_str());
		ON_SCOPE_EXIT(env) { free(env); };
		return (bool)env;
	}

	void WindowsOS::set_env(String const & key, String const & value) const
	{
		if (key.empty() || key.contains("=")) {
			CRASH("environment variable name cannot be empty or include \'=\'");
		}
		if (key.size() + value.size() + 2 > 32767) {
			CRASH("environment variable cannot exceed 32767 characters");
		}
		SetEnvironmentVariableW(key.widen().c_str(), value.widen().c_str());
	}

	void WindowsOS::unset_env(String const & key) const
	{
		if (key.empty() || key.contains("=")) {
			CRASH("environment variable name cannot be empty or include \'=\'");
		}
		SetEnvironmentVariableW(key.widen().c_str(), nullptr); // Null to delete.
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsOS::get_name() const
	{
		return "windows"_s;
	}

	String WindowsOS::get_distro() const
	{
		return {};
	}

	String WindowsOS::get_version() const
	{
		return {};
	}

	String WindowsOS::get_model_name() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<MainLoop> WindowsOS::get_main_loop() const
	{
		return m_main_loop;
	}

	void WindowsOS::set_main_loop(Ref<MainLoop> value)
	{
		m_main_loop = value;
	}

	void WindowsOS::delete_main_loop()
	{
		m_main_loop = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Date WindowsOS::get_date(bool local) const
	{
		return {};
	}

	Time WindowsOS::get_time(bool local) const
	{
		return {};
	}

	TimeZoneInfo WindowsOS::get_time_zone() const
	{
		return {};
	}

	void WindowsOS::delay(Duration const & duration)
	{
	}

	Duration WindowsOS::get_ticks() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsOS::get_locale() const
	{
		return {};
	}

	String WindowsOS::get_processor_name() const
	{
		return {};
	}

	String WindowsOS::get_unique_id() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsOS::get_cache_path() const noexcept
	{
		static String cache_path_cache;
		if (cache_path_cache.empty()) {
			if (has_env("LOCALAPPDATA")) {
				cache_path_cache = get_env("LOCALAPPDATA").replace("\\", "/");
			}
			if (cache_path_cache.empty() && has_env("TEMP")) {
				cache_path_cache = get_env("TEMP").replace("\\", "/");
			}
			if (cache_path_cache.empty()) {
				cache_path_cache = get_config_path();
			}
		}
		return cache_path_cache;
	}

	String WindowsOS::get_config_path() const noexcept
	{
		if (has_env("APPDATA")) {
			return get_env("APPDATA").replace("\\", "/");
		}
		return ".";
	}

	String WindowsOS::get_data_path() const noexcept
	{
		return get_config_path();
	}

	String WindowsOS::get_exec_path() const noexcept
	{
		WCHAR buf[4096];
		GetModuleFileNameW(nullptr, buf, 4096);
		return Unicode(buf).narrow().replace("\\", "/");
	}

	String WindowsOS::get_resource_path() const noexcept
	{
		return globals()->get_resource_path();
	}

	String WindowsOS::get_system_path(SystemDir_ value) const noexcept
	{
		return OS::get_system_path(value);
	}

	String WindowsOS::get_user_path() const noexcept
	{
		return OS::get_user_path();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ WindowsOS::move_to_trash(String const & path)
	{
		return Error_Failed;
	}

	void WindowsOS::debug_break()
	{
		__debugbreak();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}