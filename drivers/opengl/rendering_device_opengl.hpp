#if OPENGL_ENABLED
#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

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
		/* VERTEXBUFFER */
		struct _VertexBuffer
		{
			uint32_t handle{};

			Buffer data{};
		};

		virtual RID vertexbuffer_create(uint32_t size_bytes, Buffer const & data = {}) override;
		virtual void vertexbuffer_destroy(RID rid) override;
		virtual void vertexbuffer_set_data(RID rid, Buffer const & data, size_t offset = 0) override;

	public:
		/* VERTEXARRAY */
		struct _VertexArray
		{
			uint32_t handle{};

			uint32_t vertex_count{};

			VertexFormat format{};

			Vector<RID> buffers{};
		};

		virtual RID vertexarray_create(uint32_t vertex_count, VertexFormat const & format, Vector<RID> const & buffers) override;
		virtual void vertexarray_destroy(RID rid) override;

	public:
		/* INDEXBUFFER */
		struct _IndexBuffer
		{
			uint32_t handle{};

			uint32_t index_count{};

			uint32_t index_type{};

			Buffer data{};
		};

		virtual RID indexbuffer_create(uint32_t index_count, IndexbufferFormat_ index_type = IndexbufferFormat_U32, Buffer const & data = {}) override;
		virtual void indexbuffer_destroy(RID rid) override;
		virtual void indexbuffer_set_data(RID rid, Buffer const & data, size_t offset = 0) override;

	public:
		/* TEXTURE */
		struct _Texture
		{
			uint32_t handle{};

			uint32_t texture_type{};

			ColorFormat_ color_format{}, color_format_srgb{};

			ImageFormat_ image_format{};

			int32_t width{}, height{}, depth{}, layers{}, mipmaps{};

			uint32_t min_filter{}, mag_filter{};

			uint32_t repeat_s{}, repeat_t{};

			TextureSamples_ samples{};

			Vec4i swizzle_mask{};

			TextureFlags usage_flags{};

			int32_t width_2d{ width }, height_2d{ height }; // size override
		};

		virtual RID texture_create(RD::TextureFormat const & format, Buffer const & data = {}) override;
		virtual void texture_destroy(RID rid) override;
		virtual void texture_bind(RID rid, size_t slot = 0) override;
		virtual void texture_update(RID rid, Buffer const & data = {}) override;
		virtual Buffer texture_get_data(RID rid) override;
		virtual void * texture_get_handle(RID texture) override;
		void _texture_update(RID rid, void const * data);

	public:
		/* FRAMEBUFFER */
		struct _Framebuffer
		{
			uint32_t handle{};

			int32_t width{}, height{};

			Vector<RID> texture_attachments{};
		};

		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) override;
		virtual void framebuffer_destroy(RID framebuffer) override;
		virtual void framebuffer_bind(RID framebuffer) override;
		virtual void framebuffer_set_size(RID framebuffer, int32_t width, int32_t height) override;

	public:
		/* SAMPLER */
		struct _SamplerState
		{
			uint32_t handle{};

			uint32_t mag_filter{}, min_filter{}, mip_filter{};

			uint32_t repeat_s{}, repeat_t{}, repeat_r{};

			float_t lod_bias{};

			bool use_anisotropy{};

			float_t anisotropy_max{};

			bool enable_compare{};

			uint32_t compare_op{};

			float_t min_lod{}, max_lod{};

			SamplerBorderColor_ border_color{};

			bool unnormalized_uvw{};
		};

		virtual RID sampler_create(RD::SamplerState const & sampler_state) override;
		virtual void sampler_destroy(RID sampler) override;

	public:
		/* SHADER */
		struct _Shader
		{
			uint32_t handle{};

			FlatMap<hash_t, int32_t> bindings{};

			Vector<ShaderStageData> stage_data{};
		};

		virtual RID shader_create(Vector<RD::ShaderStageData> const & stage_data) override;
		virtual void shader_destroy(RID shader) override;
		virtual void shader_bind(RID shader) override;

		struct UniformBinder;
		virtual int32_t shader_uniform_location(RID shader, cstring name) override;
		virtual void shader_uniform1i(RID shader, cstring name, int32_t const value) override;
		virtual void shader_uniform1f(RID shader, cstring name, float_t const value) override;
		virtual void shader_uniform2f(RID shader, cstring name, float_t const * value) override;
		virtual void shader_uniform3f(RID shader, cstring name, float_t const * value) override;
		virtual void shader_uniform4f(RID shader, cstring name, float_t const * value) override;
		virtual void shader_uniform16f(RID shader, cstring name, float_t const * value, bool transpose = false) override;

	public:
		/* UNIFORMS */
		struct _UniformSet
		{
			RID shader{};
		};

		virtual RID uniform_buffer_create(Buffer const & data = {}) override;
		virtual RID uniform_set_create(Vector<Uniform> const & uniforms, RID shader) override;

	public:
		/* PIPELINE */
		struct _Pipeline
		{
			RID shader{};

			uint32_t primitive{};

			RD::RasterizationState rasterization_state{};
			
			RD::MultisampleState multisample_state{};
			
			RD::DepthStencilState depthstencil_state{};
			
			RD::ColorBlendState colorblend_state{};
		};

		virtual RID pipeline_create(RID shader, RenderPrimitive_ primitive, RD::RasterizationState const & rasterization_state, RD::MultisampleState const & multisample_state, RD::DepthStencilState const & depthstencil_state, RD::ColorBlendState const & colorblend_state) override;
		virtual void pipeline_destroy(RID pipeline) override;

	public:
		/* DRAW LIST */
		struct _DrawList
		{
			IntRect viewport{};

			struct State {
				RID pipeline{};
				RID pipeline_shader{};
				RID vertexarray{};
				RID indexbuffer{};
			} state{};
		}
		*m_drawlist{};

		virtual RID drawlist_begin_for_screen(WindowID window, Color const & clear_color = {}) override;
		virtual RID drawlist_begin(RID framebuffer, Color const & clear_color = {}, float_t clear_depth = 1.f, int32_t clear_stencil = 0) override;
		virtual void drawlist_bind_pipeline(RID list, RID pipeline) override;
		virtual void drawlist_bind_uniforms(RID list, RID uniforms) override;
		virtual void drawlist_bind_vertices(RID list, RID vertexarray) override;
		virtual void drawlist_bind_indices(RID list, RID indexbuffer) override;
		virtual void drawlist_draw(RID list, bool use_indices, uint32_t instances, uint32_t procedural_vertices) override;
		virtual void drawlist_end() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
