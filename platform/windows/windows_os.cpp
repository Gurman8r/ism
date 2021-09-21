#include <platform/windows/windows_os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Windows_OS::~Windows_OS() { delete_main_loop(); }

Windows_OS::Windows_OS(HINSTANCE hInstance) : OS{}, m_instance{ hInstance } {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error Windows_OS::open_dynamic_library(String const & path, void *& instance)
{
	if (path.empty()) { return Error_Unknown; }
	instance = LoadLibraryA(path.c_str());
	if (!instance) { return Error_Unknown; }
	return Error_None;
}

Error Windows_OS::close_dynamic_library(void * instance)
{
	FreeLibrary((HMODULE)instance);
	return Error_None;
}

Error Windows_OS::get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional)
{
	if (!instance || name.empty()) { return Error_Unknown; }
	symbol = GetProcAddress((HMODULE)instance, name.c_str());
	if (!symbol && !is_optional) { return Error_Unknown; }
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Windows_OS::has_environment(String const & key) const
{
	return false;
}

String Windows_OS::get_environment(String const & key) const
{
	return String{};
}

bool Windows_OS::set_environment(String const & key, String const & value) const
{
	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Windows_OS::get_name() const
{
	return String{};
}

String Windows_OS::get_stdin_string(bool block)
{
	if (block) {
		String temp;
		std::getline(std::cin, temp);
		return temp;
	}
	return String{};
}

Error Windows_OS::set_cwd(String const & path)
{
	return Error_Unknown;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Windows_OS::initialize()
{
	m_main_loop = nullptr;
}

void Windows_OS::finalize()
{
	memdelete_nonzero(m_main_loop);

	m_main_loop = nullptr;
}

void Windows_OS::finalize_core()
{
	// cleanup internals
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MainLoop * Windows_OS::get_main_loop() const
{
	return m_main_loop;
}

void Windows_OS::set_main_loop(MainLoop * value)
{
	if (m_main_loop != value) { m_main_loop = value; }
}

void Windows_OS::delete_main_loop()
{
	if (m_main_loop) { memdelete(m_main_loop); m_main_loop = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */