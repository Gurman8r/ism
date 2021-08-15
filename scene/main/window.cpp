#include <scene/main/window.hpp>
#include <servers/display_server.hpp>

#ifdef ISM_OS_WINDOWS
#include <platform/windows/windows_window.hpp>
#endif

namespace ism
{
	Window::~Window() {}

	Window * Window::new_(WindowSettings const & settings)
	{
		Window * window{ Windows_Window::new_(settings) };
		if (!window) {
			return nullptr;
		}

		DisplayServer::set_active_window(window->get_handle());

		window->set_user_pointer(window);

		//window->set_position((get_desktop_video_mode().size - settings.video.size) / 2);
		window->set_position((Vec2(1920, 1080) - settings.video.size) / 2);

		if (window->has_hints(WindowHints_Maximized)) { window->maximize(); }

		return window;
	}
}