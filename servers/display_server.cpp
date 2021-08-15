#include <servers/display_server.hpp>

#include <glfw/glfw3.h>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECLEXPR(DisplayServer::singleton) {};

	DisplayServer::~DisplayServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool DisplayServer::initialize()
	{
		return glfwInit() == GLFW_TRUE;
	}

	void DisplayServer::finalize()
	{
		glfwTerminate();
	}

	void DisplayServer::poll_events()
	{
		glfwPollEvents();
	}

	void DisplayServer::swap_buffers(WindowID value)
	{
		glfwSwapBuffers((GLFWwindow *)value);
	}

	void DisplayServer::swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	WindowID DisplayServer::get_active_window()
	{
		return (WindowID)glfwGetCurrentContext();
	}

	WindowID DisplayServer::set_active_window(WindowID value)
	{
		glfwMakeContextCurrent((GLFWwindow *)value);
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CursorID DisplayServer::create_custom_cursor(size_t w, size_t h, byte const * p, int32_t x, int32_t y)
	{
		GLFWimage temp{ (int32_t)w, (int32_t)h, (uint8_t *)p };

		return (CursorID)glfwCreateCursor(&temp, x, y);
	}

	CursorID DisplayServer::create_standard_cursor(int32_t shape)
	{
		return (CursorID)glfwCreateStandardCursor(std::invoke([shape]() noexcept
		{
			switch (shape)
			{
			default							: return 0;
			case CursorShape_Arrow			: return GLFW_ARROW_CURSOR;
			case CursorShape_IBeam			: return GLFW_IBEAM_CURSOR;
			case CursorShape_Crosshair		: return GLFW_CROSSHAIR_CURSOR;
			case CursorShape_PointingHand	: return GLFW_POINTING_HAND_CURSOR;
			case CursorShape_EW				: return GLFW_RESIZE_EW_CURSOR;
			case CursorShape_NS				: return GLFW_RESIZE_NS_CURSOR;
			case CursorShape_NESW			: return GLFW_RESIZE_NESW_CURSOR;
			case CursorShape_NWSE			: return GLFW_RESIZE_NWSE_CURSOR;
			case CursorShape_ResizeAll		: return GLFW_RESIZE_ALL_CURSOR;
			case CursorShape_NotAllowed		: return GLFW_NOT_ALLOWED_CURSOR;

			// glfw doesn't have these
			case CursorShape_HResize		: return GLFW_HRESIZE_CURSOR;
			case CursorShape_VResize		: return GLFW_VRESIZE_CURSOR;
			case CursorShape_Hand			: return GLFW_HAND_CURSOR;
			}
		}));
	}

	void DisplayServer::destroy_cursor(CursorID const & value)
	{
		glfwDestroyCursor((GLFWcursor *)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}