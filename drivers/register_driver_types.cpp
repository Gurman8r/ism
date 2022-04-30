#include <drivers/register_driver_types.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

void ism::register_driver_types()
{
	REGISTER_OBJECT_CLASS
	(
		RENDERING_DEVICE_DEFAULT
	);
}

void ism::unregister_driver_types()
{
}