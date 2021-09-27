#ifdef OPENGL_ENABLED

#include <platform/windows/context_gl_windows.hpp>

#include <dwmapi.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

#if defined(__GNUC__)
// Workaround GCC warning from -Wcast-function-type.
#define wglGetProcAddress (void *)wglGetProcAddress
#endif

typedef HGLRC(APIENTRY * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, int const *);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

ContextGL_Windows::ContextGL_Windows(HWND hwnd, bool opengl_3_context) : hWnd{ hwnd }, opengl_3_context{ opengl_3_context } {}

ContextGL_Windows::~ContextGL_Windows() {}

void ContextGL_Windows::release_current()
{
	wglMakeCurrent(hDC, nullptr);
}

void ContextGL_Windows::make_current()
{
	wglMakeCurrent(hDC, hRC);
}

int32_t ContextGL_Windows::get_window_width()
{
	return (int32_t)get_display_context().get_desktop_video_mode().size[0];
}

int32_t ContextGL_Windows::get_window_height()
{
	return (int32_t)get_display_context().get_desktop_video_mode().size[1];
}

void ContextGL_Windows::swap_buffers()
{
	SwapBuffers(hDC);

	if (use_vsync)
	{
		bool vsync_via_compositor_now{ should_vsync_via_compositor() };

		if (vsync_via_compositor_now && wglGetSwapIntervalEXT() == 0) {
			DwmFlush();
		}

		if (vsync_via_compositor_now != vsync_via_compositor) {
			// The previous frame had a different operating mode than this
			// frame.  Set the 'vsync_via_compositor' member variable and the
			// OpenGL swap interval to their proper values.
			set_use_vsync(true);
		}
	}
}

Error ContextGL_Windows::initialize()
{
	static PIXELFORMATDESCRIPTOR pfd
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1,
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
				PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
				PFD_DOUBLEBUFFER,
		(BYTE)PFD_TYPE_RGBA,
		(BYTE)24 /*(OS::get_singleton()->is_layered_allowed() ? 32 : 24*/,
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Color Bits Ignored
		(BYTE)0 /*(OS::get_singleton()->is_layered_allowed() ? 8 : 0*/, // Alpha Buffer
		(BYTE)0, // Shift Bit Ignored
		(BYTE)0, // No Accumulation Buffer
		(BYTE)0, (BYTE)0, (BYTE)0, (BYTE)0, // Accumulation Bits Ignored
		(BYTE)24, // 24Bit Z-Buffer (Depth Buffer)
		(BYTE)0, // No Stencil Buffer
		(BYTE)0, // No Auxiliary Buffer
		(BYTE)PFD_MAIN_PLANE, // Main Drawing Layer
		(BYTE)0, // Reserved
		0, 0, 0 // Layer Masks Ignored
	};

	hDC = GetDC(hWnd);
	if (!hDC) {
		return Error_Unknown;
	}

	pixel_format = ChoosePixelFormat(hDC, &pfd);
	if (!pixel_format) // Did Windows Find A Matching Pixel Format?
	{
		return Error_Unknown;
	}

	BOOL ret = SetPixelFormat(hDC, pixel_format, &pfd);
	if (!ret) // Are We Able To Set The Pixel Format?
	{
		return Error_Unknown;
	}

	hRC = wglCreateContext(hDC);
	if (!hRC) // Are We Able To Get A Rendering Context?
	{
		return Error_Unknown;
	}

	wglMakeCurrent(hDC, hRC);

	if (opengl_3_context)
	{
		int32_t attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // we want a 3.3 context
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			// and it shall be forward compatible so that we can only use up to date functionality
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB /*| _WGL_CONTEXT_DEBUG_BIT_ARB*/,
			0
		}; // zero indicates the end of the array

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr; // pointer to the method
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		if (wglCreateContextAttribsARB == nullptr) // OpenGL 3.0 is not supported
		{
			wglDeleteContext(hRC);
			return Error_Unknown;
		}

		HGLRC new_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
		if (!new_hRC) {
			wglDeleteContext(hRC);
			return Error_Unknown;
		}
		wglMakeCurrent(hDC, nullptr);
		wglDeleteContext(hRC);
		hRC = new_hRC;

		if (!wglMakeCurrent(hDC, hRC)) // Try To Activate The Rendering Context
		{
			return Error_Unknown;
		}
	}

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	//glWrapperInit(wrapper_get_proc_address);

	return Error_None;
}

void ContextGL_Windows::set_use_vsync(bool p_use)
{
	vsync_via_compositor = p_use && should_vsync_via_compositor();

	if (wglSwapIntervalEXT) {
		int32_t swap_interval = (p_use && !vsync_via_compositor) ? 1 : 0;
		wglSwapIntervalEXT(swap_interval);
	}

	use_vsync = p_use;
}

bool ContextGL_Windows::is_using_vsync() const
{
	return use_vsync;
}

bool ContextGL_Windows::should_vsync_via_compositor()
{
	//if (get_os().is_window_fullscreen() || !get_os().is_vsync_via_compositor_enabled()) {
	//	return false;
	//}

	// Note: All Windows versions supported by Godot have a compositor.
	// It can be disabled on earlier Windows versions.
	BOOL dwm_enabled;

	if (SUCCEEDED(DwmIsCompositionEnabled(&dwm_enabled))) {
		return dwm_enabled;
	}

	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
