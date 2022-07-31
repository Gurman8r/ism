#include <servers/register_server_types.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>
#include <servers/text_server.hpp>

using namespace ism;

void ism::preregister_server_types()
{
}

void ism::register_server_types()
{
	Internals::get_singleton()->initialize_class
	<
		DisplayServer,
		RenderingDevice,
		RenderingServerDefault,
		RenderingServer,
		TextServer
	>();
}

void ism::register_server_singletons()
{
}

void ism::unregister_server_types()
{
}