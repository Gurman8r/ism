#include <drivers/windows/windows_directory.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(WindowsDir, t) {};

	WindowsDir::WindowsDir() {}

	WindowsDir::~WindowsDir() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void WindowsDir::initialize()
	{
		Directory::__create_func = []() -> Ref<Directory> { return memnew(WindowsDir); };
	}

	void WindowsDir::finalize()
	{
		Directory::__create_func = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}