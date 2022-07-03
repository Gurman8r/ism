#include <core/version.hpp>

#if (OS_WINDOWS && IS_STARTUP_PROJECT)

#include <main/main.hpp>
#include <platform/windows/os_windows.hpp>

int _main()
{
	using namespace ism;

	OS_Windows os{ (HINSTANCE)nullptr };

	switch (Main::setup(__argv[0], __argc, __argv)) {
	case Error_None: break;
	case Error_Unknown:
	default: CRASH("An unknown error occurred during setup. The program was unable to start.");
	}

	ASSERT(Main::start());

	while (Main::iteration() == EXIT_SUCCESS);

	Main::cleanup();

	return os.get_exit_code();
}

int main(int argc, char * argv[])
{
	return _main();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

#endif // ISM_OS_WINDOWS
