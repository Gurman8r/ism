#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <servers/display_server.hpp>

// enums
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(TextureType)
	{
		TextureType_1D,
		TextureType_2D,
		TextureType_3D,
		TextureType_Cube,
		TextureType_1D_Array,
		TextureType_2D_Array,
		TextureType_Cube_Array,
		TextureType_Proxy_2D,
		TextureType_Proxy_1D_Array,
		TextureType_Proxy_Cubemap,
		TextureType_MAX
	};

	ENUM_INT(TextureSamples)
	{
		TextureSamples_1,
		TextureSamples_2,
		TextureSamples_4,
		TextureSamples_8,
		TextureSamples_16,
		TextureSamples_32,
		TextureSamples_64,
		TextureSamples_MAX
	};

	ENUM_INT(TextureFlags)
	{
		TextureFlags_None,
		TextureFlags_Sampling				= 1 << 0,
		TextureFlags_ColorAttachment		= 1 << 1,
		TextureFlags_DepthStencilAttachment = 1 << 2,
		TextureFlags_Storage				= 1 << 3,
		TextureFlags_Storage_Atomic			= 1 << 4,
		TextureFlags_CpuRead				= 1 << 5,
		TextureFlags_CanUpdate				= 1 << 6,
		TextureFlags_CanCopyFrom			= 1 << 7,
		TextureFlags_CanCopyTo				= 1 << 8,
		TextureFlags_ResolveAttachment		= 1 << 9,

		TextureFlags_Default =
			TextureFlags_Sampling |
			TextureFlags_CanUpdate |
			TextureFlags_CanCopyFrom
	};

	ENUM_INT(TextureSwizzle)
	{
		TextureSwizzle_Identity,
		TextureSwizzle_Zero,
		TextureSwizzle_One,
		TextureSwizzle_R,
		TextureSwizzle_G,
		TextureSwizzle_B,
		TextureSwizzle_A,
		TextureSwizzle_MAX
	};

	ENUM_INT(TextureSliceType)
	{
		TextureSliceType_2D,
		TextureSliceType_Cube,
		TextureSliceType_3D,
		TextureSliceType_2D_Array,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(SamplerFilter)
	{
		SamplerFilter_Nearest,
		SamplerFilter_Linear,
	};

	ENUM_INT(SamplerRepeatMode)
	{
		SamplerRepeatMode_Repeat,
		SamplerRepeatMode_MirroredRepeat,
		SamplerRepeatMode_ClampToEdge,
		SamplerRepeatMode_ClampToBorder,
		SamplerRepeatMode_MirrorClampToEdge,
		SamplerRepeatMode_MAX
	};

	ENUM_INT(SamplerBorderColor)
	{
		SamplerBorderColor_Float_Transparent_Black,
		SamplerBorderColor_Int_Transparent_Black,
		SamplerBorderColor_Float_Opaque_Black,
		SamplerBorderColor_Int_Opaque_Black,
		SamplerBorderColor_Float_Opaque_White,
		SamplerBorderColor_Int_Opaque_White,
		SamplerBorderColor_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(VertexFrequency)
	{
		VertexFrequency_Vertex,
		VertexFrequency_Instance,
	};

	ENUM_INT(IndexbufferFormat)
	{
		IndexbufferFormat_U16,
		IndexbufferFormat_U32,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(ShaderStage)
	{
		ShaderStage_Vertex,
		ShaderStage_Fragment,
		ShaderStage_Geometry,
		ShaderStage_TesselationControl,
		ShaderStage_TesselationEvaluation,
		ShaderStage_Compute,
		ShaderStage_MAX,
		ShaderStage_Vertex_Bit					= 1 << 0,
		ShaderStage_Fragment_Bit				= 1 << 1,
		ShaderStage_Geometry_Bit				= 1 << 2,
		ShaderStage_TesselationControl_Bit		= 1 << 3,
		ShaderStage_TesselationEvaluation_Bit	= 1 << 4,
		ShaderStage_Compute_Bit					= 1 << 5,
	};

	ENUM_INT(ShaderLanguage)
	{
		ShaderLanguage_GLSL,
		ShaderLanguage_HLSL,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(UniformType)
	{
		UniformType_Sampler,
		UniformType_SamplerWithTexture,
		UniformType_Texture,
		UniformType_Image,
		UniformType_Texture_Buffer,
		UniformType_SamplerWithTexture_Buffer,
		UniformType_Image_Buffer,
		UniformType_Uniform_Buffer,
		UniformType_Storage_Buffer,
		UniformType_InputAttachment,
		UniformType_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(RenderPrimitive)
	{
		RenderPrimitive_Points,
		RenderPrimitive_Lines,
		RenderPrimitive_LinesWithAdjacency,
		RenderPrimitive_Linestrips,
		RenderPrimitive_LinestripsWithAdjacency,
		RenderPrimitive_Triangles,
		RenderPrimitive_TrianglesWithAdjacency,
		RenderPrimitive_TriangleStrips,
		RenderPrimitive_TriangleStripsWithAdjacency,
		RenderPrimitive_TriangleStripsWithRestartIndex,
		RenderPrimitive_TesselationPatch,
		RenderPrimitive_MAX
	};

	ENUM_INT(PolygonCullMode)
	{
		PolygonCullMode_Disabled,
		PolygonCullMode_Front,
		PolygonCullMode_Back,
	};

	ENUM_INT(PolygonFrontFace)
	{
		PolygonFrontFace_Clockwise,
		PolygonFrontFace_CounterClockwise,
	};

	ENUM_INT(StencilOperation)
	{
		StencilOperation_Keep,
		StencilOperation_Zero,
		StencilOperation_Replace,
		StencilOperation_IncrementAndClamp,
		StencilOperation_DecrementAndClamp,
		StencilOperation_Invert,
		StencilOperation_IncrementAndWrap,
		StencilOperation_DecrementAndWrap,
		StencilOperation_MAX
	};

	ENUM_INT(BlendFactor)
	{
		BlendFactor_Zero,
		BlendFactor_One,
		BlendFactor_SrcColor,
		BlendFactor_OneMinusSrcColor,
		BlendFactor_DstColor,
		BlendFactor_OneMinusDstColor,
		BlendFactor_SrcAlpha,
		BlendFactor_OneMinusSrcAlpha,
		BlendFactor_DstAlpha,
		BlendFactor_OneMinusDstAlpha,
		BlendFactor_ConstantColor,
		BlendFactor_OneMinusConstantColor,
		BlendFactor_ConstantAlpha,
		BlendFactor_OneMinusConstantAlpha,
		BlendFactor_SrcAlphaSaturate,
		BlendFactor_Src1Color,
		BlendFactor_OneMinusSrc1Color,
		BlendFactor_Src1Alpha,
		BlendFactor_OneMinusSrc1Alpha,
		BlendFactor_MAX
	};

	ENUM_INT(BlendOperation)
	{
		BlendOperation_Add,
		BlendOperation_Subtract,
		BlendOperation_ReverseSubtract,
		BlendOperation_Minimum,
		BlendOperation_Maximum,
		BlendOperation_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// data
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct VertexFormat final
	{
		struct Element final
		{
			cstring		name{};
			DataType_	type{};
			uint32_t	count{};
			bool		normalized{};
			uint32_t	size{ ism::get_data_type_size(type) * count };
			uint32_t	offset{};
		};

		uint32_t stride{};

		Vector<Element> elements{};

		template <class It> VertexFormat(It first, It last) noexcept : elements{ first, last }
		{
			uint32_t offset{};
			for (Element & e : elements) {
				e.offset = offset;
				offset += e.size;
				stride += e.size;
			}
		}

		VertexFormat(std::initializer_list<Element> init) noexcept : VertexFormat{ init.begin(), init.end() } {}

		template <size_t N> VertexFormat(Element const (&arr)[N]) noexcept : VertexFormat{ &arr[0], &arr[N] } {}

		VertexFormat() noexcept : VertexFormat{
			{ "a_Position",	DataType_F32, 3 },
			{ "a_Normal",	DataType_F32, 3 },
			{ "a_Texcoord",	DataType_F32, 2 },
		} {}

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// rendering device
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// low-level graphics api
	class ISM_API RenderingDevice : public Object
	{
		OBJECT_COMMON(RenderingDevice, Object);

		static RenderingDevice * singleton;

	protected:
		explicit RenderingDevice() noexcept { singleton = this; }

	public:
		virtual ~RenderingDevice() noexcept override = default;

		NODISCARD static RenderingDevice * get_singleton() noexcept { return singleton; }

	public:
		virtual void initialize() = 0;
		virtual void finalize() = 0;

	public:
		virtual void clear(Color const & color = {}, bool depth_stencil = true) = 0;
		virtual void set_viewport(IntRect const & rect) = 0;

	public:
		/* VERTEXARRAY */
		virtual RID vertexarray_create(VertexFormat const & layout, Vector<RID> const & buffers) = 0;
		virtual void vertexarray_destroy(RID vertexarray) = 0;
		virtual void vertexarray_bind(RID vertexarray) = 0;
		virtual RID * vertexarray_get_buffers(RID vertexarray) = 0;
		virtual size_t vertexarray_get_buffer_count(RID vertexarray) = 0;

	public:
		/* VERTEXBUFFER */
		virtual RID vertexbuffer_create(Buffer const & data) = 0;
		virtual void vertexbuffer_destroy(RID vertexbuffer) = 0;
		virtual void vertexbuffer_bind(RID vertexbuffer) = 0;
		virtual void vertexbuffer_update(RID vertexbuffer, Buffer const & data, size_t offset = 0) = 0;

	public:
		/* INDEXBUFFER */
		virtual RID indexbuffer_create(Buffer const & data) = 0;
		virtual void indexbuffer_destroy(RID indexbuffer) = 0;
		virtual void indexbuffer_bind(RID indexbuffer) = 0;
		virtual void indexbuffer_update(RID indexbuffer, Buffer const & data, size_t offset = 0) = 0;

	public:
		/* TEXTURE */
		struct TextureFormat final
		{
			TextureType_ texture_type{ TextureType_2D };

			ColorFormat_ color_format{ ColorFormat_R8G8B8_UNORM };

			uint32_t width{ 1 }, height{ 1 }, depth{ 1 }, layers{ 1 }, mipmaps{ 1 };

			SamplerFilter_ min_filter{ SamplerFilter_Nearest }, mag_filter{ SamplerFilter_Nearest };

			SamplerRepeatMode_ repeat_s{ SamplerRepeatMode_Repeat }, repeat_t{ SamplerRepeatMode_Repeat };

			TextureSamples_ samples{ TextureSamples_1 };

			TextureFlags usage_flags{ TextureFlags_Default };

			ColorFormat_ color_format_srgb{ ColorFormat_MAX };

			TextureSwizzle_ swizzle_r{ TextureSwizzle_R }, swizzle_g{ TextureSwizzle_G }, swizzle_b{ TextureSwizzle_B }, swizzle_a{ TextureSwizzle_A };
		};

		virtual RID texture_create(TextureFormat const & format, Buffer const & data = {}) = 0;
		virtual void texture_destroy(RID texture) = 0;
		virtual void texture_bind(RID texture, size_t slot = 0) = 0;
		virtual void texture_update(RID texture, Buffer const & data = {}) = 0;
		virtual Buffer texture_get_data(RID texture) = 0;
		virtual RID texture_get_handle(RID texture) = 0;

	public:
		/* FRAMEBUFFER */
		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) = 0;
		virtual void framebuffer_destroy(RID framebuffer) = 0;
		virtual void framebuffer_bind(RID framebuffer) = 0;
		virtual void framebuffer_set_size(RID framebuffer, int32_t width, int32_t height) = 0;

	public:
		/* SAMPLER */
		struct SamplerState final
		{
			SamplerFilter_ mag_filter{ SamplerFilter_Nearest }, min_filter{ SamplerFilter_Nearest }, mip_filter{ SamplerFilter_Nearest };

			SamplerRepeatMode_ repeat_u{ SamplerRepeatMode_ClampToEdge }, repeat_v{ SamplerRepeatMode_ClampToEdge }, repeat_w{ SamplerRepeatMode_ClampToEdge };

			float_t lod_bias{ 0 };

			bool use_anisotropy{ false };

			float_t anisotropy_max{ 1.f };

			bool enable_compare{};

			CompareOperator_ compare_op{ CompareOperator_Always };

			float_t min_lod{ 0 }, max_lod{ 1e20f }; //something very large should do

			SamplerBorderColor_ border_color{ SamplerBorderColor_Float_Opaque_Black };

			bool unnormalized_uvw{ false };
		};

		virtual RID sampler_create(SamplerState const & sampler_state) = 0;
		virtual void sampler_destroy(RID sampler) = 0;

	public:
		/* SHADER */
		struct ShaderStageData final
		{
			ShaderStage_ shader_stage{ ShaderStage_Vertex };
			Buffer source{};
		};

		virtual RID shader_create(Vector<ShaderStageData> const & stage_data) = 0;
		virtual void shader_destroy(RID shader) = 0;
		virtual void shader_bind(RID shader) = 0;

		virtual int32_t shader_uniform_location(RID shader, cstring name) = 0;
		virtual void shader_uniform1i(RID shader, cstring name, int32_t const value) = 0;
		virtual void shader_uniform1f(RID shader, cstring name, float_t const value) = 0;
		virtual void shader_uniform2f(RID shader, cstring name, float_t const * value) = 0;
		virtual void shader_uniform3f(RID shader, cstring name, float_t const * value) = 0;
		virtual void shader_uniform4f(RID shader, cstring name, float_t const * value) = 0;
		virtual void shader_uniform16f(RID shader, cstring name, float_t const * value, bool transpose = false) = 0;

	public:
		/* UNIFORMS */
		struct Uniform final
		{
			UniformType_ uniform_type{ UniformType_Image };

			int32_t binding{};

			Vector<RID> ids{};
		};

		virtual RID uniform_buffer_create(Buffer const & data = {}) = 0;
		virtual RID uniform_set_create(Vector<Uniform> const & uniforms, RID shader) = 0;

	public:
		/* PIPELINE */
		struct RasterizationState final
		{
			bool enable_depth_clamp{ false };

			bool discard_primitives{ false };

			bool wireframe{ false };

			PolygonCullMode_ cull_mode{ PolygonCullMode_Disabled };

			PolygonFrontFace_ front_face{ PolygonFrontFace_Clockwise };

			bool depth_bias_enable{ false };

			float_t depth_bias_constant_factor{ 0.f };

			float_t depth_bias_clamp{ 0.f };

			float_t depth_bias_slope_factor{ 0.f };

			float_t line_width{ 1.f };

			uint32_t patch_control_points{ 1 };
		};

		struct MultisampleState final
		{
			TextureSamples_ sample_count{ TextureSamples_1 };

			bool enable_sample_shading{ false };

			float_t min_sample_shading{ 0.f };

			Buffer sample_mask{};

			bool enable_alpha_to_coverage{ false };

			bool enable_alpha_to_one{ false };
		};

		struct DepthStencilState final
		{
			struct StencilOperationState final
			{
				StencilOperation_ fail{ StencilOperation_Zero };

				StencilOperation_ pass{ StencilOperation_Zero };

				StencilOperation_ depth_fail{ StencilOperation_Zero };

				CompareOperator_ compare{ CompareOperator_Always };

				uint32_t compare_mask{ 0 };

				uint32_t write_mask{ 0 };

				uint32_t reference{ 0 };
			};

			bool enable_depth_test{ false };

			bool enable_depth_write{ false };

			CompareOperator_ depth_compare_operator{ CompareOperator_Always };

			bool enable_depth_range{ false };

			float_t depth_range_min{ 0.f }, depth_range_max{ 0.f };

			bool enable_stencil{ false };

			StencilOperationState front_op{}, back_op{};
		};

		struct ColorBlendState final
		{
			struct Attachment final
			{
				bool enable_blend{ false };

				BlendFactor_ src_color_blend_factor{ BlendFactor_Zero };

				BlendFactor_ dst_color_blend_factor{ BlendFactor_Zero };

				BlendOperation_ color_blend_op{ BlendOperation_Add };

				BlendFactor_ src_alpha_blend_factor{ BlendFactor_Zero };

				BlendFactor_ dst_alpha_blend_factor{ BlendFactor_Zero };

				BlendOperation_ alpha_blend_op{ BlendOperation_Add };

				bool write_r{ true }, write_g{ true }, write_b{ true }, write_a{ true };
			};

			bool enable_logic_op{ false };
			
			LogicOperation_ logic_op{ LogicOperation_Clear };
			
			Vector<Attachment> attachments{};
			
			Color blend_constant{};

			NODISCARD static ColorBlendState create_disabled(int32_t num_attachments = 1)
			{
				ColorBlendState temp{};
				temp.attachments.reserve(num_attachments);
				for (int32_t i = 0; i < num_attachments; i++) {
					temp.attachments.push_back(Attachment{});
				}
				return temp;
			}

			NODISCARD static ColorBlendState create_blend(int32_t num_attachments = 1)
			{
				ColorBlendState temp{};
				temp.attachments.reserve(num_attachments);
				for (int32_t i = 0; i < num_attachments; i++) {
					temp.attachments.push_back(Attachment{ true,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add, });
				}
				return temp;
			}
		};

		virtual RID render_pipeline_create(RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depthstencil_state, ColorBlendState const & colorblend_state) = 0;
		virtual void render_pipeline_destroy(RID pipeline) = 0;
		
	public:
		/* DRAW LIST */
		virtual RID drawlist_begin_for_screen(WindowID window, Color const & clear_color = {}) = 0;
		virtual RID drawlist_begin(RID framebuffer, Color const & clear_color = {}, float_t clear_depth = 1.f, int32_t clear_stencil = 0) = 0;
		virtual void drawlist_bind_pipeline(RID list, RID pipeline) = 0;
		virtual void drawlist_bind_uniforms(RID list, RID uniforms) = 0;
		virtual void drawlist_bind_vertices(RID list, RID vertexarray) = 0;
		virtual void drawlist_bind_indices(RID list, RID indexbuffer) = 0;
		virtual void drawlist_draw(RID list, bool use_indices, uint32_t instances, uint32_t procedural_vertices) = 0;
		virtual void drawlist_end() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(RD) RenderingDevice;

#define RENDERING_DEVICE (ism::RenderingDevice::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
