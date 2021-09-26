#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJ_CLASS(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();

		RenderingDeviceOpenGL();

	public: /* TEXTURE */

		virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<Vector<byte>> const & data = {}) override;

	public: /* FRAMEBUFFER */

		virtual RID framebuffer_create(Vector<RID> const & texture_attachments, FramebufferFormatID format_check = -1) override;

	public: /* BUFFER */

		virtual VertexFormatID vertex_format_create(Vector<VertexAttribute> const & vertex_formats) override;

		virtual RID vertex_array_create(uint32_t size_bytes, Vector<VertexAttribute> const & vertex_format, Vector<RID> const & src_buffers) override;

		virtual RID vertex_buffer_create(uint32_t size_bytes, Vector<byte> const & data = {}, bool use_as_storage = false) override;

		virtual RID index_buffer_create(uint32_t size_indices, IndexBufferFormat format, Vector<byte> const & data = {}, bool use_restart_indices = false) override;
		
	public: /* SHADER */

		virtual RID shader_create(Vector<ShaderStageData> const & stages) override;

	public: /* SCREEN */

		virtual int32_t screen_get_width(WindowID window = 0) const override;

		virtual int32_t screen_get_height(WindowID window = 0) const override;

		virtual FramebufferFormatID screen_get_framebuffer_format() const override;

	public: /* MISC */

		virtual void free(RID id) override;
	};
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
