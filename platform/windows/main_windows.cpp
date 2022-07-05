#include <core/version.hpp>

#if (SYSTEM_WINDOWS && MAIN_ENABLED)

#include <main/main.hpp>
#include <platform/windows/os_windows.hpp>

int _main()
{
	ism::OS_Windows os{ NULL };

	switch (ism::Main::setup(__argv[0], __argc, __argv)) {
	case ism::Error_None: break;
	case ism::Error_Unknown:
	default: {
		CRASH("An unknown error occurred during setup. The program was unable to start.");
	} break;
	}

	ASSERT(ism::Main::start());

	while (ism::Main::iteration() == EXIT_SUCCESS);

	ism::Main::cleanup();

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

#endif // SYSTEM_WINDOWS
