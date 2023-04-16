#include <platform/windows/os_windows.hpp>
#include <drivers/windows/file_access_windows.hpp>
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
		m_main_loop = nullptr;

		FileAccessWindows::initialize();
	}

	void OS_Windows::finalize()
	{
		m_main_loop = nullptr;
	}

	void OS_Windows::finalize_core()
	{
		FileAccessWindows::finalize();
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

	Error_ OS_Windows::open_dynamic_library(Path const & path, void *& instance)
	{
		if (!path) { return Error_Unknown; }
		instance = LoadLibraryA(path.c_str());
		if (!instance) { return Error_Unknown; }
		return Error_None;
	}

	Error_ OS_Windows::close_dynamic_library(void * instance)
	{
		FreeLibrary((HMODULE)instance);
		return Error_None;
	}

	Error_ OS_Windows::get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional)
	{
		if (!instance || name.empty()) { return Error_Unknown; }
		symbol = GetProcAddress((HMODULE)instance, name.c_str());
		if (!symbol && !is_optional) { return Error_Unknown; }
		return Error_None;
	}

	Error_ OS_Windows::execute(Path const & path, Vector<String> const & args, String * pipe, i32 * exitcode, bool read_stderr, Mutex * pipe_mutex)
	{
		return Error_Unknown;
	}

	Error_ OS_Windows::create_process(Path const & path, Vector<String> const & args, ProcessID * child_id)
	{
		return Error_Unknown;
	}

	Error_ OS_Windows::kill(ProcessID const & pid)
	{
		return Error_Unknown;
	}

	i32 OS_Windows::get_process_id() const
	{
		return -1;
	}

	Path OS_Windows::get_cwd() const
	{
		return (Path)(String)std::filesystem::current_path().string();
	}

	Error_ OS_Windows::set_cwd(Path const & path)
	{
		return std::filesystem::current_path((std::wstring)path.string().widen()), Error_None;
	}

	Error_ OS_Windows::shell_open(Path const & path)
	{
		return Error_Unknown;
	}

	Path OS_Windows::get_env(String const & key) const
	{
		return ""_path;
	}

	bool OS_Windows::has_env(String const & key) const
	{
		return false;
	}

	bool OS_Windows::set_env(String const & key, Path const & value) const
	{
		return false;
	}

	String OS_Windows::get_name() const
	{
		return "Windows"_s;
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