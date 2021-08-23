#include <drivers/opengl/opengl_device.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(OpenGL_RenderingDevice, t, "opengl_rendering_device")
{
	t.tp_base = typeof<RenderingDevice>();
}

OpenGL_RenderingDevice::~OpenGL_RenderingDevice()
{
}

OpenGL_RenderingDevice::OpenGL_RenderingDevice() : RenderingDevice{}
{
}