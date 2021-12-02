#include <servers/register_server_types.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>

using namespace ism;

void ism::register_server_types()
{
	SINGLETON(Internals)->bind_class<
		DisplayServer,
		RenderingDevice,
		RenderingServerDefault,
		RenderingServer
	>();
}

void ism::unregister_server_types()
{
}