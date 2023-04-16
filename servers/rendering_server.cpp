#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderingServer * RenderingServer::__singleton{};

	EMBED_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}

	RenderingServer::RenderingServer() { __singleton = this; }

	RenderingServer::~RenderingServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}