#include <drivers/register_driver_types.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

namespace ism
{
	void register_driver_types()
	{
		Internals::get_singleton()->initialize_class
		<
			RENDERING_DEVICE_DEFAULT
		>();
	}

	void unregister_driver_types()
	{
	}
}