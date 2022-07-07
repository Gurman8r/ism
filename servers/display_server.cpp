#include <servers/display_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MEMBER_IMPL(DisplayServer::g_singleton) {};

	OBJECT_EMBED(DisplayServer, t, TypeFlags_IsAbstract) {}

	DisplayServer::DisplayServer() { g_singleton = this; }

	DisplayServer::~DisplayServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}