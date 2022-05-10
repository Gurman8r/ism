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
		/* BUFFER API */
		struct BufferBase
		{
			uint32_t handle{};
			uint32_t buffer_type{};
			uint32_t usage{};
			uint32_t size{};
			uint32_t draw_mode{};
			DynamicBuffer data{};
		};

		virtual RID buffer_create(BufferType_ buffer_type, size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual void buffer_destroy(RID buffer) override;
		virtual void buffer_update(RID buffer, size_t offset, void const * data, size_t size_in_bytes) override;

	public:
		/* VERTEXARRAY API */
		struct VertexBuffer : BufferBase {};

		struct VertexArray
		{
			uint32_t handle{};
			uint32_t vertex_count{};
			VertexLayout layout{};
			Vector<RID> buffers{};
		};

		struct IndexBuffer : BufferBase
		{
			uint32_t index_count{};
			uint32_t index_type{};
		};

		struct IndexArray
		{
			RID index_buffer{};
			uint32_t index_offset{};
			uint32_t index_count{};
			uint32_t index_type{};
		};

		virtual RID vertex_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual RID vertex_array_create(size_t vertex_count, VertexLayout const & layout, Vector<RID> const & buffers) override;
		virtual void vertex_array_destroy(RID rid) override;

		virtual RID index_buffer_create(size_t index_count, IndexbufferFormat_ index_type = IndexbufferFormat_U32, DynamicBuffer const & data = {}) override;
		virtual RID index_array_create(RID index_buffer, size_t index_offset, size_t index_count) override;
		virtual void index_array_destroy(RID index_array) override;

	public:
		/* SAMPLER API */
		struct Sampler
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

		virtual RID sampler_create(SamplerCreateInfo const & sampler_state = {}) override;
		virtual void sampler_destroy(RID sampler) override;

	public:
		/* TEXTURE API */
		struct Texture
		{
			uint32_t handle{};
			uint32_t texture_type{};
			DataFormat_ color_format{}, color_format_srgb{};
			Image::Format_ image_format{};
			int32_t width{}, height{}, depth{}, layers{}, mipmaps{};
			uint32_t min_filter{}, mag_filter{};
			uint32_t repeat_s{}, repeat_t{};
			TextureSamples_ samples{};
			Vec4i swizzle_mask{};
			int32_t usage_flags{};
			int32_t width_2d{ width }, height_2d{ height }; // size override
		};

		virtual RID texture_create(TextureCreateInfo const & spec, DynamicBuffer const & data = {}) override;
		virtual void texture_destroy(RID rid) override;
		virtual void texture_update(RID rid, DynamicBuffer const & data = {}) override;
		void _texture_update(RID rid, void const * data);
		virtual void * texture_get_handle(RID texture) override;
		virtual DynamicBuffer texture_get_data(RID rid) override;

	public:
		/* FRAMEBUFFER API */
		struct Framebuffer
		{
			uint32_t handle{};
			int32_t width{}, height{};
			Vector<RID> texture_attachments{};
		};

		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) override;
		virtual void framebuffer_destroy(RID framebuffer) override;
		virtual void framebuffer_set_size(RID framebuffer, int32_t width, int32_t height) override;

	public:
		/* SHADER API */
		struct Shader
		{
			uint32_t handle{};
			FlatMap<hash_t, int32_t> bindings{};
			Vector<ShaderStageData> stage_data{};
		};

		virtual RID shader_create(ShaderCreateInfo const & spec) override;
		virtual void shader_destroy(RID shader) override;

	public:
		/* UNIFORM API */
		struct UniformBuffer final : BufferBase {};

		struct UniformDescriptor final
		{
			UniformType_ uniform_type{};
			uint32_t binding{};
			uint32_t length{};
			Vector<RID> buffers{};
			Vector<RID> textures{};
		};

		struct UniformSet final
		{
			RID shader{};
			Vector<UniformDescriptor> uniforms{};
		};

		virtual RID uniform_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual RID uniform_set_create(Vector<Uniform> const & uniforms, RID shader) override;
		virtual void uniform_set_destroy(RID uniform_set) override;

	public:
		/* PIPELINE API */
		struct RenderPipeline final
		{
			RID shader{};
			uint32_t primitive{};
			RasterizationState rasterization_state{};
			MultisampleState multisample_state{};
			DepthStencilState depth_stencil_state{};
			ColorBlendState color_blend_state{};
		};

		virtual RID pipeline_create(RID shader, RenderPrimitive_ primitive, RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depth_stencil_state, ColorBlendState const & color_blend_state) override;
		virtual void pipeline_destroy(RID pipeline) override;

	public:
		/* DRAWLIST API */
		struct DrawList final
		{
			IntRect viewport{};

			struct State final
			{
				struct SetState final {
					RID uniform_set{};
					bool bound{ true };
				} sets[32]{};
				uint32_t set_count{};

				RID pipeline{};
				RID pipeline_shader{};
				RID vertex_array{};
				RID index_array{};
			}
			state{};
		};

		Vector<DrawList> m_lists{};

		virtual RID draw_list_begin_for_screen(WindowID window, Color const & clear_color = {}) override;
		virtual RID draw_list_begin(RID framebuffer, Color const & clear_color = {}, float_t clear_depth = 1.f, int32_t clear_stencil = 0) override;
		virtual void draw_list_bind_pipeline(RID draw_list, RID pipeline) override;
		virtual void draw_list_bind_uniform_set(RID draw_list, RID uniform_set, size_t index) override;
		virtual void draw_list_bind_vertex_array(RID draw_list, RID vertex_array) override;
		virtual void draw_list_bind_index_array(RID draw_list, RID index_array) override;
		virtual void draw_list_draw(RID draw_list, bool use_indices, size_t instances, size_t procedural_vertices) override;
		virtual void draw_list_end() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
