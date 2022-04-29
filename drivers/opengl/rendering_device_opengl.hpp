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
		virtual RID vertexarray_create(VertexLayout const & layout, RID indices, RID vertices) override;
		virtual void vertexarray_destroy(RID rid) override;
		virtual void vertexarray_bind(RID rid) override;
		virtual void vertexarray_draw(RID rid) override;

	public:
		/* VERTEXBUFFER */
		virtual RID vertexbuffer_create(Buffer const & data) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_bind(RID rid) override;
		virtual void vertexbuffer_update(RID rid, Buffer const & data, size_t offset = 0) override;

	public:
		/* INDEXBUFFER */
		virtual RID indexbuffer_create(Buffer const & data) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_bind(RID rid) override;
		virtual void indexbuffer_update(RID rid, Buffer const & data, size_t offset = 0) override;

	public:
		/* TEXTURE */
		virtual RID texture_create(TextureFormat const & format, Buffer const & data = {}) override;
		virtual void texture_destroy(RID rid) override;
		virtual void texture_bind(RID rid, size_t slot = 0) override;
		virtual void texture_update(RID rid, Buffer const & data = {}) override;
		virtual Buffer texture_get_data(RID rid) override;
		void _texture_update(RID rid, void const * data);

	public:
		/* FRAMEBUFFER */
		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) override;
		virtual void framebuffer_destroy(RID rid) override;
		virtual void framebuffer_bind(RID rid) override;
		virtual void framebuffer_resize(RID rid, int32_t width, int32_t height) override;

	public:
		/* SHADER */
		virtual RID shader_create(Vector<ShaderStageData> const & stage_data) override;
		virtual void shader_destroy(RID rid) override;
		virtual void shader_bind(RID rid) override;
		virtual int32_t shader_uniform_location(RID rid, cstring name) override;
		virtual void shader_set_uniform1i(RID rid, cstring name, int32_t const value) override;
		virtual void shader_set_uniform1f(RID rid, cstring name, float_t const value) override;
		virtual void shader_set_uniform2f(RID rid, cstring name, Vec2 const & value) override;
		virtual void shader_set_uniform3f(RID rid, cstring name, Vec3 const & value) override;
		virtual void shader_set_uniform4f(RID rid, cstring name, Vec4 const & value) override;
		virtual void shader_set_uniform16f(RID rid, cstring name, Mat4 const & value, bool transpose = false) override;

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
