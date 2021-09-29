#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

using FramebufferFormatID = RenderingDevice::FramebufferFormatID;
using VertexFormatID = RenderingDevice::VertexFormatID;
using VertexAttribute = RenderingDevice::VertexAttribute;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{} {}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */