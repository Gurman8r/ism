#include <core/version.hpp>

#ifdef ISM_OS_WINDOWS

#include <platform/windows/os_windows.hpp>

extern int main(int, char * []);

OS_IMPL(void * user)
{
	RETURN_STATIC(ism::OS_Windows{ user ? (HINSTANCE)user : GetModuleHandle(NULL) });
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	OS_MAIN(hInstance);

	return main(__argc, __argv);
}

#endif // ISM_OS_WINDOWS
