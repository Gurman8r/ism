#include <core/version.hpp>

#ifdef ISM_OS_WINDOWS

#include <platform/windows/windows_os.hpp>

extern int main(int, char * []);

ISM_IMPLEMENTATION(void * hInstance)
{
	RETURN_STATIC(ism::Windows_OS{ hInstance ? (HINSTANCE)hInstance : GetModuleHandle(NULL) });
}

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow)
{
	IMPLEMENT_ISM(hInstance);

	return main(__argc, __argv);
}

#endif // ISM_OS_WINDOWS
