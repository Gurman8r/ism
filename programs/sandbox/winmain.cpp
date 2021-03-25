#include <core/language_features.hpp>

#ifdef ISM_OS_WINDOWS

#include <platform/windows/os_windows.hpp>

extern INT main(INT, CHAR * []);

ISM::OS const * create_os(void * instance = {})
{
	static ISM::OS_Windows os{ instance ? (HINSTANCE)instance : GetModuleHandle(NULL) };
	return std::addressof(os);
}

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow)
{
	create_os(hInstance);
	return main(__argc, __argv);
}

#endif // ISM_OS_WINDOWS
