#include <platform/windows/os_windows.hpp>
#include <platform/windows/display_server_windows.hpp>
#include <drivers/windows/file_access_windows.hpp>
#include <main/main.hpp>
#include <filesystem>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OS_Windows::OS_Windows(HINSTANCE hInstance) : OS{}
	{
		m_hinstance = hInstance ? hInstance : GetModuleHandle(NULL);
	}

	OS_Windows::~OS_Windows()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS_Windows::initialize()
	{
		FileAccessWindows::initialize();
		DisplayServerWindows::initialize();
	}

	void OS_Windows::initialize_joysticks()
	{
	}

	void OS_Windows::finalize()
	{
	}

	void OS_Windows::finalize_core()
	{
		DisplayServerWindows::finalize();
		FileAccessWindows::finalize();
	}

	void OS_Windows::run()
	{
		if (!m_main_loop) { return; }
		m_main_loop->initialize();
		while (true) {
			DISPLAY_SERVER->poll_events();
			if (Main::iteration()) { break; }
			DISPLAY_SERVER->swap_buffers();
		}
		m_main_loop->finalize();
	}

	Vector<String> OS_Windows::get_video_adapter_driver_info() const
	{
		return {};
	}

	String OS_Windows::get_stdin_string(bool blocking)
	{
		if (blocking) {
			String temp;
			std::getline(std::cin, temp);
			return temp;
		}
		return {};
	}

	Error_ OS_Windows::open_dynamic_library(String const & path, void *& instance)
	{
		if (path.empty()) { return Error_Unknown; }
		instance = LoadLibraryA(path.c_str());
		if (!instance) { return Error_Unknown; }
		return Error_OK;
	}

	Error_ OS_Windows::close_dynamic_library(void * instance)
	{
		if (!instance) { return Error_Unknown; }
		FreeLibrary((HMODULE)instance);
		return Error_OK;
	}

	Error_ OS_Windows::get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional)
	{
		if (!instance || name.empty()) { return Error_Unknown; }
		symbol = GetProcAddress((HMODULE)instance, name.c_str());
		if (!symbol && !is_optional) { return Error_Unknown; }
		return Error_OK;
	}

	Error_ OS_Windows::execute(String const & path, Vector<String> const & args, String * pipe, i32 * exitcode, bool read_stderr, Mutex * pipe_mutex)
	{
		return Error_Unknown;
	}

	Error_ OS_Windows::create_process(String const & path, Vector<String> const & args, ProcessID * child_id)
	{
		return Error_Unknown;
	}

	Error_ OS_Windows::kill(ProcessID const & pid)
	{
		return Error_Unknown;
	}

	i32 OS_Windows::get_pid() const
	{
		return -1;
	}

	String OS_Windows::get_cwd() const
	{
		return (String)std::filesystem::current_path().string();
	}

	Error_ OS_Windows::set_cwd(String const & path)
	{
		return std::filesystem::current_path((std::wstring)path.widen()), Error_OK;
	}

	Error_ OS_Windows::shell_open(String const & path)
	{
		return Error_Unknown;
	}

	String OS_Windows::get_env(String const & key) const
	{
		return "";
	}

	bool OS_Windows::has_env(String const & key) const
	{
		return false;
	}

	bool OS_Windows::set_env(String const & key, String const & value) const
	{
		return false;
	}

	String OS_Windows::get_name() const
	{
		return "windows"_s;
	}

	String OS_Windows::get_model_name() const
	{
		return ""_s;
	}

	Ref<MainLoop> OS_Windows::get_main_loop() const
	{
		return m_main_loop;
	}

	void OS_Windows::set_main_loop(Ref<MainLoop> value)
	{
		m_main_loop = value;
	}

	void OS_Windows::delete_main_loop()
	{
		m_main_loop = nullptr;
	}

	OS::Date OS_Windows::get_date(bool local) const
	{
		return Date{};
	}

	OS::Time OS_Windows::get_time(bool local) const
	{
		return Time{};
	}

	OS::TimeZoneInfo OS_Windows::get_time_zone() const
	{
		return TimeZoneInfo{};
	}

	void OS_Windows::debug_break()
	{
		_CSTD __debugbreak();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}