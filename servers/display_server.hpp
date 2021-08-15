#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <scene/main/window.hpp>

namespace ism
{
	class ISM_API DisplayServer : public Super
	{
		ISM_SUPER(DisplayServer, Super);

		static DisplayServer * singleton;

	public:
		virtual ~DisplayServer();

		static DisplayServer * get_singleton() { return singleton; }

	public:
		static bool initialize();

		static void finalize();

		static void poll_events();

		static void swap_buffers(WindowID value);

		static void swap_interval(int32_t value);

		static WindowID get_active_window();

		static WindowID set_active_window(WindowID value);

	public:
		static CursorID create_custom_cursor(size_t w, size_t h, byte const * p, int32_t x, int32_t y);

		static CursorID create_standard_cursor(int32_t shape);

		static void destroy_cursor(CursorID const & value);
	};

	inline DisplayServer & get_display_server() noexcept { return *CHECK(DisplayServer::get_singleton()); }
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
