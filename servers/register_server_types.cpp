#include <servers/register_server_types.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

void ism::register_server_types()
{
	SINGLETON(Internals)->bind_class<
		DisplayServer,
		RenderingServer
	>();
}

void ism::unregister_server_types()
{
}