#if OPENGL_ENABLED
#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <drivers/opengl/opengl.hpp>
#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJECT_COMMON(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();
		RenderingDeviceOpenGL();

	public:
		virtual void initialize() override;
		virtual void finalize() override;

	public:
		virtual void clear(Color const & color = {}, bool depth_stencil = true) override;
		virtual void set_viewport(IntRect const & rect) override;

	public:
		/* VERTEXARRAY */
		virtual RID vertexarray_create() override;
		virtual void vertexarray_destroy(RID rid) override;
		virtual void vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices) override;

	public:
		/* VERTEXBUFFER */
		virtual RID vertexbuffer_create(Vector<byte> const & data) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		/* INDEXBUFFER */
		virtual RID indexbuffer_create(Vector<byte> const & data) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		/* TEXTURE */
		virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data) override;
		virtual void texture_destroy(RID rid) override;
		virtual void texture_update(RID rid, Vector<byte> const & data) override;
		virtual void texture_set_size_override(RID rid, int32_t width, int32_t height) override;

	public:
		/* FRAMEBUFFER */
		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) override;
		virtual void framebuffer_destroy(RID rid) override;

	public:
		/* SHADER */
		virtual RID shader_create(Vector<ShaderStageData> const & stage_data) override;
		virtual void shader_destroy(RID rid) override;

	protected:
		Batch<
			RD_VertexArray,
			RD_VertexBuffer,
			RD_IndexBuffer,
			RD_Texture,
			RD_FrameBuffer,
			RD_Shader
		> m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
