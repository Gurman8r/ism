#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

using FramebufferFormatID = RenderingDevice::FramebufferFormatID;
using VertexFormatID = RenderingDevice::VertexFormatID;
using VertexAttribute = RenderingDevice::VertexAttribute;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{} {}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureFormat const & format, TextureView const & view, Vector<Vector<byte>> const & data)
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments, FramebufferFormatID format_check)
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

VertexFormatID RenderingDeviceOpenGL::vertex_format_create(Vector<VertexAttribute> const & vertex_formats)
{
	return 0;
}

RID RenderingDeviceOpenGL::vertex_array_create(uint32_t size_bytes, Vector<VertexAttribute> const & vertex_format, Vector<RID> const & src_buffers)
{
	return 0;
}

RID RenderingDeviceOpenGL::vertex_buffer_create(uint32_t size_bytes, Vector<byte> const & data, bool use_as_storage)
{
	return 0;
}

RID RenderingDeviceOpenGL::index_buffer_create(uint32_t size_indices, IndexBufferFormat format, Vector<byte> const & data, bool use_restart_indices)
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(Vector<ShaderStageData> const & stages)
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t RenderingDeviceOpenGL::screen_get_width(WindowID window) const
{
	return 0;
}

int32_t RenderingDeviceOpenGL::screen_get_height(WindowID window) const
{
	return 0;
}

FramebufferFormatID RenderingDeviceOpenGL::screen_get_framebuffer_format() const
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::free(RID id)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */