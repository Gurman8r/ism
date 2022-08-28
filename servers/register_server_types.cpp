#include <servers/register_server_types.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>
#include <servers/text_server.hpp>

namespace ism
{
	void register_server_types()
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

	void register_server_singletons()
	{
	}

	void unregister_server_types()
	{
	}
}