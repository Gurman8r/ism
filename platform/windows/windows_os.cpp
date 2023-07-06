#include <platform/windows/windows_os.hpp>
#include <platform/windows/windows_display_server.hpp>
#include <drivers/windows/windows_dir.hpp>
#include <drivers/windows/windows_file.hpp>
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
			get_display()->poll_events();
			if (Main::iteration()) { break; }
			get_display()->swap_buffers();
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
		return {};
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
	}

	void WindowsOS::unset_env(String const & key) const
	{
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

	String WindowsOS::get_bin_dir() const noexcept
	{
		return OS::get_bin_dir();
	}

	String WindowsOS::get_cache_dir() const noexcept
	{
		return OS::get_cache_dir();
	}

	String WindowsOS::get_config_dir() const noexcept
	{
		return OS::get_config_dir();
	}

	String WindowsOS::get_data_dir() const noexcept
	{
		return OS::get_data_dir();
	}

	String WindowsOS::get_downloads_dir() const noexcept
	{
		return OS::get_downloads_dir();
	}

	String WindowsOS::get_exe_dir() const noexcept
	{
		WCHAR buf[4096];
		GetModuleFileNameW(nullptr, buf, 4096);
		return Unicode(buf).narrow().replace("\\", "/");
	}

	String WindowsOS::get_mods_dir() const noexcept
	{
		return OS::get_mods_dir();
	}

	String WindowsOS::get_profiles_dir() const noexcept
	{
		return OS::get_profiles_dir();
	}

	String WindowsOS::get_resource_dir() const noexcept
	{
		return OS::get_resource_dir();
	}

	String WindowsOS::get_saves_dir() const noexcept
	{
		return OS::get_saves_dir();
	}

	String WindowsOS::get_system_dir(SystemDir_ value) const noexcept
	{
		return OS::get_system_dir(value);
	}

	String WindowsOS::get_user_dir() const noexcept
	{
		return OS::get_user_dir();
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