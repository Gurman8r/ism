#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/os/os.hpp>
#include <core/input/input.hpp>
#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// display server
	class ISM_API DisplayServer : public Object
	{
		OBJECT_COMMON(DisplayServer, Object);

		static DisplayServer * g_singleton;

	protected:
		DisplayServer();

	public:
		virtual ~DisplayServer() override;

		FORCE_INLINE static DisplayServer * get_singleton() noexcept { return g_singleton; }

	public:
		enum WindowMode_
		{
			WindowMode_Windowed,
			WindowMode_Minimized,
			WindowMode_Maximized,
			WindowMode_Fullscreen,
			WindowMode_MAX
		};

		struct VideoMode
		{
			Vec2i	size{ 1280, 720 };
			Vec4b	bits_per_pixel{ 8, 8, 8, 8 };
			int32_t	refresh_rate{ -1 };

			bool operator==(VideoMode const & value) const
			{
				return (this == std::addressof(value))
					|| (size == value.size
					&& bits_per_pixel == value.bits_per_pixel
					&& refresh_rate == value.refresh_rate);
			}

			bool operator<(VideoMode const & value) const
			{
				return (this != std::addressof(value))
					&& (size < value.size
					|| bits_per_pixel < value.bits_per_pixel
					|| refresh_rate < value.refresh_rate);
			}
		};

		virtual VideoMode const & get_desktop_video_mode() const = 0;
		virtual List<VideoMode> const & get_fullscreen_video_modes() const = 0;

		DECL_HANDLE(MonitorID);
		DECL_HANDLE(WindowID);

		struct Monitor
		{
			String name{};
			Vec2i size{};
			WindowID window{};
			List<VideoMode> modes{};
			VideoMode * current_mode{};
		};

		virtual WindowID get_current_context() const = 0;
		virtual void set_current_context(WindowID window) = 0;

		virtual bool window_should_close(WindowID window) const = 0;
		virtual void window_set_should_close(WindowID window, bool value) = 0;

		virtual String get_clipboard() const = 0;
		virtual void set_clipboard(String const & value) = 0;

		enum MouseMode_
		{
			MouseMode_Normal,
			MouseMode_Hidden,
			MouseMode_Disabled,
			MouseMode_MAX
		};

		virtual MouseMode_ mouse_get_mode() const = 0;
		virtual void mouse_set_mode(MouseMode_ value) = 0;

		virtual int32_t mouse_get_button(MouseButton_ value) const = 0;
		virtual Vec2 mouse_get_position() const = 0;
		virtual void mouse_set_position(Vec2 const & value) = 0;

		virtual String window_get_title(WindowID window) const = 0;
		virtual void window_set_title(WindowID window, String const & value) = 0;

		virtual MonitorID window_get_monitor(WindowID window) const = 0;
		virtual void window_set_monitor(WindowID window, MonitorID value, IntRect const & rect, int32_t refresh_rate = -1) = 0;

		virtual Vec2i window_get_position(WindowID window) const = 0;
		virtual void window_set_position(WindowID window, Vec2i const & value) = 0;

		virtual Vec2i window_get_size(WindowID window) const = 0;
		virtual void window_set_size(WindowID window, Vec2i const & value) = 0;

		virtual Vec2i window_get_real_size(WindowID window) const = 0;

		virtual WindowMode_ window_get_mode(WindowID window) const = 0;
		virtual void window_set_mode(WindowID window, WindowMode_ value) = 0;

		virtual bool window_get_flag(WindowID window, int32_t flag) const = 0;
		virtual void window_set_flag(WindowID window, int32_t flag, bool enabled) = 0;

		virtual void request_window_attention(WindowID window) = 0;
		virtual void move_window_to_foreground(WindowID window) = 0;

		virtual void window_set_visible(WindowID window, bool value) = 0;
		virtual bool window_is_visible(WindowID window) const = 0;

		virtual Vec2 window_get_content_scale(WindowID window) const = 0;

		virtual void window_grab_focus(WindowID window) = 0;
		virtual bool window_has_focus(WindowID window) const = 0;

		enum CursorShape_
		{
			CursorShape_Arrow,
			CursorShape_IBeam,
			CursorShape_Crosshair,
			CursorShape_PointingHand,
			CursorShape_EW,
			CursorShape_NS,
			CursorShape_NESW,
			CursorShape_NWSE,
			CursorShape_ResizeAll,
			CursorShape_NotAllowed,
			CursorShape_HResize,
			CursorShape_VResize,
			CursorShape_Hand,
			CursorShape_MAX
		};

		virtual CursorShape_ cursor_get_shape() const = 0;
		virtual void cursor_set_shape(CursorShape_ value) = 0;
		virtual void cursor_set_custom_image(RES const & cursor, CursorShape_ shape = {}, Vec2 const & hotspot = {}) = 0;

		virtual void poll_events() = 0;
		virtual void swap_buffers() = 0;

		virtual void set_native_icon(Path const & value) = 0;
		virtual void set_icon(Ref<Image> const & value) = 0;

		void vsync_set_enabled(bool value) {}
		bool vsync_is_enabled() const { return false; }

	public:
		using ErrorCallback =				void(*)(int32_t, cstring);
		using CharCallback =				void(*)(WindowID, uint32_t);
		using CharModsCallback =			void(*)(WindowID, uint32_t, int32_t);
		using CloseCallback =				void(*)(WindowID);
		using ContentScaleCallback =		void(*)(WindowID, float_t, float_t);
		using DropCallback =				void(*)(WindowID, int32_t, cstring[]);
		using FocusCallback =				void(*)(WindowID, int32_t);
		using FramebufferResizeCallback =	void(*)(WindowID, int32_t, int32_t);
		using IconifyCallback =				void(*)(WindowID, int32_t);
		using KeyCallback =					void(*)(WindowID, int32_t, int32_t, int32_t, int32_t);
		using MaximizeCallback =			void(*)(WindowID, int32_t);
		using MouseButtonCallback =			void(*)(WindowID, int32_t, int32_t, int32_t);
		using MouseEnterCallback =			void(*)(WindowID, int32_t);
		using MousePositionCallback =		void(*)(WindowID, double_t, double_t);
		using PositionCallback =			void(*)(WindowID, int32_t, int32_t);
		using RefreshCallback =				void(*)(WindowID);
		using SizeCallback =				void(*)(WindowID, int32_t, int32_t);
		using ScrollCallback =				void(*)(WindowID, double_t, double_t);
		using MonitorCallback =				void(*)(int32_t);
		using JoystickCallback =			void(*)(int32_t, int32_t);

		virtual ErrorCallback set_error_callback(ErrorCallback callback) = 0;
		virtual CharCallback window_set_char_callback(WindowID window, CharCallback callback) = 0;
		virtual CharModsCallback window_set_char_mods_callback(WindowID window, CharModsCallback callback) = 0;
		virtual CloseCallback window_set_close_callback(WindowID window, CloseCallback callback) = 0;
		virtual ContentScaleCallback window_set_content_scale_callback(WindowID window, ContentScaleCallback callback) = 0;
		virtual DropCallback window_set_drop_callback(WindowID window, DropCallback callback) = 0;
		virtual FocusCallback window_set_focus_callback(WindowID window, FocusCallback callback) = 0;
		virtual FramebufferResizeCallback window_set_framebuffer_resize_callback(WindowID window, FramebufferResizeCallback callback) = 0;
		virtual IconifyCallback window_set_iconify_callback(WindowID window, IconifyCallback callback) = 0;
		virtual KeyCallback window_set_key_callback(WindowID window, KeyCallback callback) = 0;
		virtual MaximizeCallback window_set_maximize_callback(WindowID window, MaximizeCallback callback) = 0;
		virtual MouseButtonCallback window_set_mouse_button_callback(WindowID window, MouseButtonCallback callback) = 0;
		virtual MouseEnterCallback window_set_mouse_enter_callback(WindowID window, MouseEnterCallback callback) = 0;
		virtual MousePositionCallback window_set_mouse_position_callback(WindowID window, MousePositionCallback callback) = 0;
		virtual PositionCallback window_set_position_callback(WindowID window, PositionCallback callback) = 0;
		virtual RefreshCallback window_set_refresh_callback(WindowID window, RefreshCallback callback) = 0;
		virtual ScrollCallback window_set_scroll_callback(WindowID window, ScrollCallback callback) = 0;
		virtual SizeCallback window_set_size_callback(WindowID window, SizeCallback callback) = 0;
		virtual MonitorCallback set_monitor_callback(MonitorCallback callback) = 0;
		virtual JoystickCallback set_joystick_callback(JoystickCallback callback) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(DS) DisplayServer;

#define DISPLAY_SERVER (ism::DisplayServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
