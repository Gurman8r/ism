#include <servers/register_server_types.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>
#include <servers/text_server.hpp>

using namespace ism;

void ism::register_server_types()
{
	REGISTER_OBJECT_CLASS(
		DisplayServer,
		RenderingDevice,
		RenderingServerDefault,
		RenderingServer,
		TextServer
	);
}

void ism::unregister_server_types()
{
}