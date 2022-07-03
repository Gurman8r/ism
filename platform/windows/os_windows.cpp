#include <platform/windows/os_windows.hpp>
#include <filesystem>

using namespace ism;

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
}

void OS_Windows::finalize()
{
	m_main_loop = nullptr;
}

void OS_Windows::finalize_core()
{
}

void OS_Windows::set_main_loop(Ref<MainLoop> value)
{
	m_main_loop = value;
}

void OS_Windows::delete_main_loop()
{
	m_main_loop = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String OS_Windows::get_name() const
{
	return "Windows"_s;
}

String OS_Windows::get_stdin_string(bool block)
{
	if (block) {
		String temp;
		std::getline(std::cin, temp);
		return temp;
	}
	return {};
}

String OS_Windows::get_environment(String const & key) const
{
	return String{};
}

bool OS_Windows::has_environment(String const & key) const
{
	return false;
}

bool OS_Windows::set_environment(String const & key, String const & value) const
{
	return false;
}

Path OS_Windows::get_cwd() const
{
	return (Path)(String)std::filesystem::current_path().string();
}

Error_ OS_Windows::set_cwd(Path const & path)
{
	return std::filesystem::current_path((std::wstring)util::widen(path.string())), Error_None;
}

Ref<MainLoop> OS_Windows::get_main_loop() const
{
	return m_main_loop;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ OS_Windows::open_dynamic_library(Path const & path, void *& instance)
{
	if (path.empty()) { return Error_Unknown; }
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */