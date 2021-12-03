#if OPENGL_ENABLED
#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <drivers/opengl/opengl.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJECT_COMMON(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();

		RenderingDeviceOpenGL();

		virtual void initialize() override;

		virtual void finalize() override;

	public:
		virtual void clear(Color const & color = {}, bool depth_stencil = true) override;

		virtual void set_viewport(IntRect const & rect) override;

	public:
		enum {
			VA_RID,
			VA_PRIMITIVE,
			VA_LAYOUT,
			VA_INDICES,
			VA_VERTICES,
		};

		Batch<RID, RenderPrimitive_, VertexLayout, RID, Vector<RID>
		> m_vertexarray{};

		virtual RID vertexarray_create(RenderPrimitive_ primitive) override;
		virtual void vertexarray_destroy(RID rid) override;
		virtual void vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices) override;

	public:
		enum {
			BUFFER_RID,
			BUFFER_USAGE,
			BUFFER_DATA,
		};

		Batch<RID, BufferUsage_, Vector<byte>
		> m_buffer{};

		virtual RID vertexbuffer_create(BufferUsage_ usage, Vector<byte> const & data) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

		virtual RID indexbuffer_create(BufferUsage_ usage, Vector<byte> const & data) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		enum {
			FB_RID,
			FB_COLOR,
			FB_DEPTH,
		};

		Batch<RID, Vector<RID>, RID
		> m_framebuffer{};

		virtual RID framebuffer_create(Vector<RID> const & attachments) override;
		virtual void framebuffer_destroy(RID rid) override;

	public:
		enum {
			TEX_RID,
			TEX_FORMAT,
			TEX_VIEW,
			TEX_DATA,
		};

		Batch<RID, TextureFormat, TextureView, Vector<byte>
		> m_texture{};

		virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data) override;
		virtual void texture_destroy(RID rid) override;

	public:
		enum {
			SHADER_RID,
			SHADER_CODE,
		};

		Batch<RID, String
		> m_shader{};

		virtual RID shader_create(Vector<ShaderStageData> const & stages) override;
		virtual void shader_destroy(RID rid) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
