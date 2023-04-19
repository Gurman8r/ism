#include <servers/rendering_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}

	RenderingServer * RenderingServer::__singleton{};

	RenderingServer::CreateFunc RenderingServer::__create_func{ []() { return (RenderingServer *)memnew(RenderingServerDefault); } };

	RenderingServer::RenderingServer()
	{
		__singleton = this;
	}

	RenderingServer::~RenderingServer()
	{
	}

	RenderingServer * RenderingServer::create()
	{
		return VALIDATE(__create_func)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}