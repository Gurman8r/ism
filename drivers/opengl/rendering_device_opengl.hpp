#if OPENGL_ENABLED
#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <drivers/opengl/opengl.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// rendering device structure
#define RD_STRUCT(m_struct)										\
public:															\
	DEFAULT_COPYABLE_MOVABLE(m_struct);							\
																\
	NODISCARD bool operator==(m_struct const & other) const {	\
		return this == std::addressof(other);					\
	}															\
																\
public:

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
		
		struct RD_VertexArray
		{
			RD_STRUCT(RD_VertexArray);

			RID handle{};
		};

		virtual RID vertexarray_create() override;
		virtual void vertexarray_destroy(RID rid) override;
		virtual void vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices) override;

	public:
		/* VERTEXBUFFER */
		
		struct RD_VertexBuffer
		{
			RD_STRUCT(RD_VertexBuffer);

			RID handle{};
		};

		virtual RID vertexbuffer_create(Vector<byte> const & data) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		/* INDEXBUFFER */
		
		struct RD_IndexBuffer
		{
			RD_STRUCT(RD_IndexBuffer);

			RID handle{};
		};

		virtual RID indexbuffer_create(Vector<byte> const & data) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset = 0) override;

	public:
		/* TEXTURE */

		struct RD_Texture
		{
			RD_STRUCT(RD_Texture);

			RID				handle{};
			TextureType_	texture_type{};
			DataFormat_		data_format{};
			TextureView		view{};

			int32_t width{}, height{}, mipmaps{};
			
			int32_t width_2d{ width }, height_2d{ height }; // size override
		};

		virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data) override;
		virtual void texture_destroy(RID rid) override;
		virtual void texture_update(RID rid, Vector<byte> const & data) override;
		virtual void texture_set_size_override(RID rid, int32_t width, int32_t height) override;

	public:
		/* FRAMEBUFFER */
		
		struct RD_FrameBuffer
		{
			RD_STRUCT(RD_FrameBuffer);

			RID handle{};
		};

		virtual RID framebuffer_create(Vector<RID> const & attachments) override;
		virtual void framebuffer_destroy(RID rid) override;

	public:
		/* SHADER */
		
		struct RD_Shader
		{
			RD_STRUCT(RD_Shader);

			RID handle{};
		};

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
