#include <platform/windows/os_windows.hpp>
#include <servers/display_server.hpp>
#include <filesystem>

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

String OS_Windows::get_cwd() const
{
	return (String)std::filesystem::current_path().string();
}

Error OS_Windows::set_cwd(String const & path)
{
	return std::filesystem::current_path(path), Error_Unknown;
}

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

VideoMode const & OS_Windows::get_desktop_video_mode() const
{
	static VideoMode result{};
	if (static bool once{}; !once && (once = true))
	{
		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);

		Vec2 resolution{ dm.dmPelsWidth, dm.dmPelsHeight };

		result = VideoMode{ resolution, Vec4{}, -1 };
	}
	return result;
}

Vector<VideoMode> const & OS_Windows::get_fullscreen_video_modes() const
{
	static Vector<VideoMode> result{};
	if (static bool once{}; !once && (once = true))
	{
		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
		{
			Vec2 resolution{ dm.dmPelsWidth, dm.dmPelsHeight };

			VideoMode vm{ resolution, Vec4{}, -1 };

			if (!ism::has(result, vm))
			{
				result.push_back(vm);
			}
		}
	}

	return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */