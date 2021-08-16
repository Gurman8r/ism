#include <servers/display_server.hpp>

using namespace ism;

DECLEXPR(ism::DisplayServer::singleton) {};

VideoMode const & ism::get_desktop_video_mode()
{
	static VideoMode temp{};
	// TODO...
	return temp;
}

Vector<VideoMode> const & ism::get_fullscreen_video_modes()
{
	static Vector<VideoMode> temp{};
	// TODO...
	return temp;
}