#include <servers/rendering_server.hpp>
#include <servers/rendering/default_rendering_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(RenderingServer, t, TypeFlags_IsAbstract) {}

	SINGLETON_EMBED(RenderingServer);

	RenderingServer::CreateFunc RenderingServer::__create_func{ []() { return (RenderingServer *)memnew(DefaultRenderingServer); } };

	RenderingServer::RenderingServer() { SINGLETON_CTOR(); }

	RenderingServer::~RenderingServer() { SINGLETON_DTOR(); }

	RenderingServer * RenderingServer::create()
	{
		return VALIDATE(__create_func)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}