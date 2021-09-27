#ifdef OPENGL_ENABLED
#ifndef _ISM_CONTEXT_GL_WINDOWS_HPP_
#define _ISM_CONTEXT_GL_WINDOWS_HPP_

#include <runtime/window/display_context.hpp>
#include <platform/windows/windows.hpp>

typedef bool(APIENTRY * PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef int(APIENTRY * PFNWGLGETSWAPINTERVALEXTPROC)(void);

namespace ism
{
	class ISM_API ContextGL_Windows
	{
	public:
		ContextGL_Windows(HWND hwnd, bool opengl_3_context);

		~ContextGL_Windows();

		void release_current();
		
		void make_current();

		int32_t get_window_width();
		
		int32_t get_window_height();
		
		void swap_buffers();

		Error initialize();

		void set_use_vsync(bool p_use);
		
		bool is_using_vsync() const;

	private:
		HDC			hDC{};
		HGLRC		hRC{};
		uint32_t	pixel_format{};
		HWND		hWnd{};
		bool		opengl_3_context{};
		bool		use_vsync{};
		bool		vsync_via_compositor{};

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT{};
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT{};

		static bool should_vsync_via_compositor();
	};
}

#endif // !_ISM_CONTEXT_GL_WINDOWS_HPP_
#endif // OPENGL_ENABLED