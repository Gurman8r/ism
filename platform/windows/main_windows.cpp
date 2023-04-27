#if MAIN_ENABLED
#include <core/version.hpp>
#if SYSTEM_WINDOWS
#include <main/main.hpp>
#include <platform/windows/os_windows.hpp>

#pragma comment(linker, "/manifestdependency:\"name='binaries' version='1.0.0.0' type='win32'\"")

int _main()
{
	ism::OS_Windows os{ (HINSTANCE)nullptr };

	switch (ism::Main::setup(__argv[0], __argc, __argv)) {
	case ism::Error_OK: break;
	// etc...
	case ism::Error_Unknown:
	default: {
		CRASH("An unknown error occurred during setup. The program was unable to start.");
	} break;
	}

	if (ism::Main::start()) { os.run(); }

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

#endif // !SYSTEM_WINDOWS
#endif // !MAIN_ENABLED