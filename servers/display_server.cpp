#include <servers/display_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DisplayServer * DisplayServer::__singleton{};

	EMBED_CLASS(DisplayServer, t, TypeFlags_IsAbstract) {}

	DisplayServer::DisplayServer()
	{
		__singleton = this;

		Input::m_funcs.get_mouse_mode = []() -> Input::MouseMode_
		{
			return DS::get_singleton()->mouse_get_mode();
		};

		Input::m_funcs.set_mouse_mode = [](Input::MouseMode_ mode)
		{
			DS::get_singleton()->mouse_set_mode(mode);
		};

		Input::m_funcs.set_mouse_position = [](Vec2 const & position)
		{
			DS::get_singleton()->mouse_set_position(position);
		};

		Input::m_funcs.get_cursor_shape = []() -> Input::CursorShape_
		{
			return DS::get_singleton()->cursor_get_shape();
		};

		Input::m_funcs.set_cursor_shape = [](Input::CursorShape_ shape)
		{
			DS::get_singleton()->cursor_set_shape(shape);
		};

		Input::m_funcs.set_custom_cursor_image = [](RES const & cursor, auto shape, Vec2 const & hotspot)
		{
			DS::get_singleton()->cursor_set_custom_image(cursor, shape, hotspot);
		};
	}

	DisplayServer::~DisplayServer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServer::vsync_set_enabled(bool value)
	{
	}

	bool DisplayServer::vsync_is_enabled() const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}