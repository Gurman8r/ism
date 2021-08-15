#include <scene/main/viewport.hpp>

namespace ism
{
	VideoMode const & get_desktop_video_mode()
	{
		static VideoMode mode{};
		// TODO...
		return mode;
	}

	Vector<VideoMode> const & get_fullscreen_video_modes()
	{
		static Vector<VideoMode> modes{};
		// TODO...
		return modes;
	}
}

namespace ism
{
	Viewport::~Viewport() {}
}