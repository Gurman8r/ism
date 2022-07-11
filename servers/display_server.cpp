#include <servers/display_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS * DS::__singleton{};

	OBJECT_EMBED(DisplayServer, t, TypeFlags_IsAbstract) {}

	DisplayServer::DisplayServer() { __singleton = this; }

	DisplayServer::~DisplayServer() {}

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