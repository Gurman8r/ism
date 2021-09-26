#include <platform/windows/os_windows.hpp>
#include <servers/display_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OS_Windows::OS_Windows(HINSTANCE hInstance) : OS{}, m_instance{ hInstance }
{
}

OS_Windows::~OS_Windows()
{
	delete_main_loop();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error OS_Windows::open_dynamic_library(String const & path, void *& instance)
{
	if (path.empty()) { return Error_Unknown; }
	instance = LoadLibraryA(path.c_str());
	if (!instance) { return Error_Unknown; }
	return Error_None;
}

Error OS_Windows::close_dynamic_library(void * instance)
{
	FreeLibrary((HMODULE)instance);
	return Error_None;
}

Error OS_Windows::get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional)
{
	if (!instance || name.empty()) { return Error_Unknown; }
	symbol = GetProcAddress((HMODULE)instance, name.c_str());
	if (!symbol && !is_optional) { return Error_Unknown; }
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool OS_Windows::has_environment(String const & key) const
{
	return false;
}

String OS_Windows::get_environment(String const & key) const
{
	return String{};
}

bool OS_Windows::set_environment(String const & key, String const & value) const
{
	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String OS_Windows::get_name() const
{
	return String{};
}

String OS_Windows::get_stdin_string(bool block)
{
	if (block) {
		String temp;
		std::getline(std::cin, temp);
		return temp;
	}
	return String{};
}

Error OS_Windows::set_cwd(String const & path)
{
	return Error_Unknown;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void OS_Windows::initialize()
{
	m_main_loop = nullptr;
}

void OS_Windows::finalize()
{
	memdelete_nonzero(m_main_loop);

	m_main_loop = nullptr;
}

void OS_Windows::finalize_core()
{
	// cleanup internals
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MainLoop * OS_Windows::get_main_loop() const
{
	return m_main_loop;
}

void OS_Windows::set_main_loop(MainLoop * value)
{
	if (m_main_loop != value) { m_main_loop = value; }
}

void OS_Windows::delete_main_loop()
{
	if (m_main_loop) { memdelete(m_main_loop); m_main_loop = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */