#include <servers/register_server_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <servers/display_server.hpp>
#include <servers/rendering/default_rendering_server.hpp>
#include <servers/text_server.hpp>

namespace Ism
{
	void register_server_types()
	{
		REGISTER_CLASS
		(
			DisplayServer,
			RenderingDevice,
			DefaultRenderingServer,
			RenderingServer,
			TextServer
		);
	}

	void register_server_singletons()
	{
	}

	void unregister_server_types()
	{
	}
}