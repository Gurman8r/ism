#include <core/language_features.hpp>

#ifdef ISM_OS_WINDOWS

#include <platform/windows/os_windows.hpp>

extern INT main(INT, CHAR * []);

ism::OS & __ism_init(void * instance)
{
	RETURN_STATIC(ism::OS_Windows(instance ? (HINSTANCE)instance : GetModuleHandle(NULL)));
}

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow)
{
	__ism_init(hInstance);

	return main(__argc, __argv);
}

#endif // ISM_OS_WINDOWS
