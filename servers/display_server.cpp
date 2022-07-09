#include <servers/display_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS * DS::__singleton{};

	OBJECT_EMBED(DisplayServer, t, TypeFlags_IsAbstract) {}

	DisplayServer::DisplayServer() { __singleton = this; }

	DisplayServer::~DisplayServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}