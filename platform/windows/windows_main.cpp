#if MAIN_ENABLED
#include <core/language_features.hpp>

#if SYSTEM_WINDOWS
#include <main/main.hpp>
#include <platform/windows/windows_os.hpp>

#pragma comment(linker, "/manifestdependency:\"name='bin' version='1.0.0.0' type='win32'\"")

int _main()
{
	using namespace Ism;

	WindowsOS os{ (HINSTANCE)nullptr };

	switch (Main::setup(__argv[0], __argc, __argv)) {
	case Error_OK: break;
	/* TODO: additional error checking goes here */
	case Error_Failed:
	default: {
		CRASH("An error occurred during setup and program was unable to start.");
	} break;
	}

	if (Main::start()) { os.run(); }

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

#endif // !SYSTEM_WINDOWS
#endif // !MAIN_ENABLED