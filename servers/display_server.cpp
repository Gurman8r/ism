#include <servers/display_server.hpp>

using namespace ism;

MEMBER_IMPL(DisplayServer::singleton) {};

OBJ_CLASS_IMPL(DisplayServer, t, "display_server", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}

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