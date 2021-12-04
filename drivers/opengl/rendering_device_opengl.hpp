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

	public:
		virtual void initialize() override;
		virtual void finalize() override;

	public:
		virtual void clear(Color const & color = {}, bool depth_stencil = true) override;
		virtual void set_viewport(IntRect const & rect) override;

	public:
		virtual RID vertexarray_create() override;
		virtual void vertexarray_destroy(RID rid) override;
		virtual void vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices) override;

	public:
		virtual RID vertexbuffer_create(Vector<byte> const & data) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		virtual RID indexbuffer_create(Vector<byte> const & data) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		virtual RID framebuffer_create(Vector<RID> const & attachments) override;
		virtual void framebuffer_destroy(RID rid) override;

	public:
		enum TEX_ {
			TEX_RID,
			TEX_FORMAT,
			TEX_VIEW
		};

		Batch<RID, TextureFormat, TextureView
		> m_texture{};

		virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data) override;
		virtual void texture_update(RID rid, Vector<byte> const & data) override;
		virtual void texture_destroy(RID rid) override;

	public:
		virtual RID shader_create(Vector<ShaderStageData> const & stages) override;
		virtual void shader_destroy(RID rid) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
