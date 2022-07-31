#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RS * RS::__singleton{};

	EMBED_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}

	RenderingServer::RenderingServer() { __singleton = this; }

	RenderingServer::~RenderingServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}