#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

using FramebufferFormatID = RenderingDevice::FramebufferFormatID;
using VertexFormatID = RenderingDevice::VertexFormatID;
using VertexAttribute = RenderingDevice::VertexAttribute;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{} {}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */