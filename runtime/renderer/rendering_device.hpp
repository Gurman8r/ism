#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <runtime/window/display_context.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// rendering device
	class ISM_API RenderingDevice : public Object
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		OBJECT_COMMON(RenderingDevice, Object);

		static RenderingDevice * singleton;

	public:
		explicit RenderingDevice() noexcept : Object{} { singleton = this; }

		virtual ~RenderingDevice() noexcept override = default;

		NODISCARD static RenderingDevice * get_singleton() noexcept { return singleton; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* GENERIC */

		MAKE_ENUM(CompareOperator)
		{
			CompareOperator_Never,
			CompareOperator_Less,
			CompareOperator_Equal,
			CompareOperator_LessOrEqual,
			CompareOperator_Greater,
			CompareOperator_NotEqual,
			CompareOperator_GreaterOrEqual,
			CompareOperator_Always,
			CompareOperator_MAX
		};

		MAKE_ENUM(DataFormat)
		{
			DataFormat_R4G4_UNORM_PACK8,
			DataFormat_R4G4B4A4_UNORM_PACK16,
			DataFormat_B4G4R4A4_UNORM_PACK16,
			DataFormat_R5G6B5_UNORM_PACK16,
			DataFormat_B5G6R5_UNORM_PACK16,
			DataFormat_R5G5B5A1_UNORM_PACK16,
			DataFormat_B5G5R5A1_UNORM_PACK16,
			DataFormat_A1R5G5B5_UNORM_PACK16,
			DataFormat_R8_UNORM,
			DataFormat_R8_SNORM,
			DataFormat_R8_USCALED,
			DataFormat_R8_SSCALED,
			DataFormat_R8_UINT,
			DataFormat_R8_SINT,
			DataFormat_R8_SRGB,
			DataFormat_R8G8_UNORM,
			DataFormat_R8G8_SNORM,
			DataFormat_R8G8_USCALED,
			DataFormat_R8G8_SSCALED,
			DataFormat_R8G8_UINT,
			DataFormat_R8G8_SINT,
			DataFormat_R8G8_SRGB,
			DataFormat_R8G8B8_UNORM,
			DataFormat_R8G8B8_SNORM,
			DataFormat_R8G8B8_USCALED,
			DataFormat_R8G8B8_SSCALED,
			DataFormat_R8G8B8_UINT,
			DataFormat_R8G8B8_SINT,
			DataFormat_R8G8B8_SRGB,
			DataFormat_B8G8R8_UNORM,
			DataFormat_B8G8R8_SNORM,
			DataFormat_B8G8R8_USCALED,
			DataFormat_B8G8R8_SSCALED,
			DataFormat_B8G8R8_UINT,
			DataFormat_B8G8R8_SINT,
			DataFormat_B8G8R8_SRGB,
			DataFormat_R8G8B8A8_UNORM,
			DataFormat_R8G8B8A8_SNORM,
			DataFormat_R8G8B8A8_USCALED,
			DataFormat_R8G8B8A8_SSCALED,
			DataFormat_R8G8B8A8_UINT,
			DataFormat_R8G8B8A8_SINT,
			DataFormat_R8G8B8A8_SRGB,
			DataFormat_B8G8R8A8_UNORM,
			DataFormat_B8G8R8A8_SNORM,
			DataFormat_B8G8R8A8_USCALED,
			DataFormat_B8G8R8A8_SSCALED,
			DataFormat_B8G8R8A8_UINT,
			DataFormat_B8G8R8A8_SINT,
			DataFormat_B8G8R8A8_SRGB,
			DataFormat_A8B8G8R8_UNORM_PACK32,
			DataFormat_A8B8G8R8_SNORM_PACK32,
			DataFormat_A8B8G8R8_USCALED_PACK32,
			DataFormat_A8B8G8R8_SSCALED_PACK32,
			DataFormat_A8B8G8R8_UINT_PACK32,
			DataFormat_A8B8G8R8_SINT_PACK32,
			DataFormat_A8B8G8R8_SRGB_PACK32,
			DataFormat_A2R10G10B10_UNORM_PACK32,
			DataFormat_A2R10G10B10_SNORM_PACK32,
			DataFormat_A2R10G10B10_USCALED_PACK32,
			DataFormat_A2R10G10B10_SSCALED_PACK32,
			DataFormat_A2R10G10B10_UINT_PACK32,
			DataFormat_A2R10G10B10_SINT_PACK32,
			DataFormat_A2B10G10R10_UNORM_PACK32,
			DataFormat_A2B10G10R10_SNORM_PACK32,
			DataFormat_A2B10G10R10_USCALED_PACK32,
			DataFormat_A2B10G10R10_SSCALED_PACK32,
			DataFormat_A2B10G10R10_UINT_PACK32,
			DataFormat_A2B10G10R10_SINT_PACK32,
			DataFormat_R16_UNORM,
			DataFormat_R16_SNORM,
			DataFormat_R16_USCALED,
			DataFormat_R16_SSCALED,
			DataFormat_R16_UINT,
			DataFormat_R16_SINT,
			DataFormat_R16_SFLOAT,
			DataFormat_R16G16_UNORM,
			DataFormat_R16G16_SNORM,
			DataFormat_R16G16_USCALED,
			DataFormat_R16G16_SSCALED,
			DataFormat_R16G16_UINT,
			DataFormat_R16G16_SINT,
			DataFormat_R16G16_SFLOAT,
			DataFormat_R16G16B16_UNORM,
			DataFormat_R16G16B16_SNORM,
			DataFormat_R16G16B16_USCALED,
			DataFormat_R16G16B16_SSCALED,
			DataFormat_R16G16B16_UINT,
			DataFormat_R16G16B16_SINT,
			DataFormat_R16G16B16_SFLOAT,
			DataFormat_R16G16B16A16_UNORM,
			DataFormat_R16G16B16A16_SNORM,
			DataFormat_R16G16B16A16_USCALED,
			DataFormat_R16G16B16A16_SSCALED,
			DataFormat_R16G16B16A16_UINT,
			DataFormat_R16G16B16A16_SINT,
			DataFormat_R16G16B16A16_SFLOAT,
			DataFormat_R32_UINT,
			DataFormat_R32_SINT,
			DataFormat_R32_SFLOAT,
			DataFormat_R32G32_UINT,
			DataFormat_R32G32_SINT,
			DataFormat_R32G32_SFLOAT,
			DataFormat_R32G32B32_UINT,
			DataFormat_R32G32B32_SINT,
			DataFormat_R32G32B32_SFLOAT,
			DataFormat_R32G32B32A32_UINT,
			DataFormat_R32G32B32A32_SINT,
			DataFormat_R32G32B32A32_SFLOAT,
			DataFormat_R64_UINT,
			DataFormat_R64_SINT,
			DataFormat_R64_SFLOAT,
			DataFormat_R64G64_UINT,
			DataFormat_R64G64_SINT,
			DataFormat_R64G64_SFLOAT,
			DataFormat_R64G64B64_UINT,
			DataFormat_R64G64B64_SINT,
			DataFormat_R64G64B64_SFLOAT,
			DataFormat_R64G64B64A64_UINT,
			DataFormat_R64G64B64A64_SINT,
			DataFormat_R64G64B64A64_SFLOAT,
			DataFormat_B10G11R11_UFLOAT_PACK32,
			DataFormat_E5B9G9R9_UFLOAT_PACK32,
			DataFormat_D16_UNORM,
			DataFormat_X8_D24_UNORM_PACK32,
			DataFormat_D32_SFLOAT,
			DataFormat_S8_UINT,
			DataFormat_D16_UNORM_S8_UINT,
			DataFormat_D24_UNORM_S8_UINT,
			DataFormat_D32_SFLOAT_S8_UINT,
			DataFormat_BC1_RGB_UNORM_BLOCK,
			DataFormat_BC1_RGB_SRGB_BLOCK,
			DataFormat_BC1_RGBA_UNORM_BLOCK,
			DataFormat_BC1_RGBA_SRGB_BLOCK,
			DataFormat_BC2_UNORM_BLOCK,
			DataFormat_BC2_SRGB_BLOCK,
			DataFormat_BC3_UNORM_BLOCK,
			DataFormat_BC3_SRGB_BLOCK,
			DataFormat_BC4_UNORM_BLOCK,
			DataFormat_BC4_SNORM_BLOCK,
			DataFormat_BC5_UNORM_BLOCK,
			DataFormat_BC5_SNORM_BLOCK,
			DataFormat_BC6H_UFLOAT_BLOCK,
			DataFormat_BC6H_SFLOAT_BLOCK,
			DataFormat_BC7_UNORM_BLOCK,
			DataFormat_BC7_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8A1_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8A1_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8A8_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8A8_SRGB_BLOCK,
			DataFormat_EAC_R11_UNORM_BLOCK,
			DataFormat_EAC_R11_SNORM_BLOCK,
			DataFormat_EAC_R11G11_UNORM_BLOCK,
			DataFormat_EAC_R11G11_SNORM_BLOCK,
			DataFormat_ASTC_4x4_UNORM_BLOCK,
			DataFormat_ASTC_4x4_SRGB_BLOCK,
			DataFormat_ASTC_5x4_UNORM_BLOCK,
			DataFormat_ASTC_5x4_SRGB_BLOCK,
			DataFormat_ASTC_5x5_UNORM_BLOCK,
			DataFormat_ASTC_5x5_SRGB_BLOCK,
			DataFormat_ASTC_6x5_UNORM_BLOCK,
			DataFormat_ASTC_6x5_SRGB_BLOCK,
			DataFormat_ASTC_6x6_UNORM_BLOCK,
			DataFormat_ASTC_6x6_SRGB_BLOCK,
			DataFormat_ASTC_8x5_UNORM_BLOCK,
			DataFormat_ASTC_8x5_SRGB_BLOCK,
			DataFormat_ASTC_8x6_UNORM_BLOCK,
			DataFormat_ASTC_8x6_SRGB_BLOCK,
			DataFormat_ASTC_8x8_UNORM_BLOCK,
			DataFormat_ASTC_8x8_SRGB_BLOCK,
			DataFormat_ASTC_10x5_UNORM_BLOCK,
			DataFormat_ASTC_10x5_SRGB_BLOCK,
			DataFormat_ASTC_10x6_UNORM_BLOCK,
			DataFormat_ASTC_10x6_SRGB_BLOCK,
			DataFormat_ASTC_10x8_UNORM_BLOCK,
			DataFormat_ASTC_10x8_SRGB_BLOCK,
			DataFormat_ASTC_10x10_UNORM_BLOCK,
			DataFormat_ASTC_10x10_SRGB_BLOCK,
			DataFormat_ASTC_12x10_UNORM_BLOCK,
			DataFormat_ASTC_12x10_SRGB_BLOCK,
			DataFormat_ASTC_12x12_UNORM_BLOCK,
			DataFormat_ASTC_12x12_SRGB_BLOCK,
			DataFormat_G8B8G8R8_422_UNORM,
			DataFormat_B8G8R8G8_422_UNORM,
			DataFormat_G8_B8_R8_3PLANE_420_UNORM,
			DataFormat_G8_B8R8_2PLANE_420_UNORM,
			DataFormat_G8_B8_R8_3PLANE_422_UNORM,
			DataFormat_G8_B8R8_2PLANE_422_UNORM,
			DataFormat_G8_B8_R8_3PLANE_444_UNORM,
			DataFormat_R10X6_UNORM_PACK16,
			DataFormat_R10X6G10X6_UNORM_2PACK16,
			DataFormat_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
			DataFormat_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
			DataFormat_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
			DataFormat_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
			DataFormat_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
			DataFormat_R12X4_UNORM_PACK16,
			DataFormat_R12X4G12X4_UNORM_2PACK16,
			DataFormat_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
			DataFormat_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
			DataFormat_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
			DataFormat_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
			DataFormat_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
			DataFormat_G16B16G16R16_422_UNORM,
			DataFormat_B16G16R16G16_422_UNORM,
			DataFormat_G16_B16_R16_3PLANE_420_UNORM,
			DataFormat_G16_B16R16_2PLANE_420_UNORM,
			DataFormat_G16_B16_R16_3PLANE_422_UNORM,
			DataFormat_G16_B16R16_2PLANE_422_UNORM,
			DataFormat_G16_B16_R16_3PLANE_444_UNORM,
			DataFormat_PVRTC1_2BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC1_4BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC2_2BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC2_4BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC1_2BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC1_4BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC2_2BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC2_4BPP_SRGB_BLOCK_IMG,
			DataFormat_MAX
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* TEXTURE */

		MAKE_ENUM(TextureType)
		{
			TextureType_1D,
			TextureType_2D,
			TextureType_3D,
			TextureType_Cube,
			TextureType_1D_Array,
			TextureType_2D_Array,
			TextureType_Cube_Array,
			TextureType_MAX
		};

		MAKE_ENUM(TextureSamples)
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

		MAKE_ENUM(TextureUsage)
		{
			TextureUsage_Sampling				= 1 << 0,
			TextureUsage_ColorAttachment		= 1 << 1,
			TextureUsage_DepthStencilAttachment = 1 << 2,
			TextureUsage_Storage				= 1 << 3,
			TextureUsage_Storage_Atomic			= 1 << 4,
			TextureUsage_CpuRead				= 1 << 5,
			TextureUsage_CanUpdate				= 1 << 6,
			TextureUsage_CanCopyFrom			= 1 << 7,
			TextureUsage_CanCopyTo				= 1 << 8,
			TextureUsage_ResolveAttachment		= 1 << 9,
		};

		MAKE_ENUM(TextureSwizzle)
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

		struct NODISCARD TextureFormat final
		{
			DEFAULT_COPYABLE_MOVABLE(TextureFormat);

			DataFormat format{ DataFormat_R8_UNORM };
			
			uint32_t width{ 1 };
			
			uint32_t height{ 1 };
			
			uint32_t depth{ 1 };
			
			uint32_t array_layers{ 1 };
			
			uint32_t mipmaps{ 1 };
			
			TextureType texture_type{ TextureType_2D };
			
			TextureSamples samples{ TextureSamples_1 };
			
			uint32_t usage_bits{ 0 };
		};

		struct NODISCARD TextureView final
		{
			DEFAULT_COPYABLE_MOVABLE(TextureView);

			DataFormat format_override{ DataFormat_MAX };
			
			TextureSwizzle swizzle_r{ TextureSwizzle_R };
			
			TextureSwizzle swizzle_g{ TextureSwizzle_G };
			
			TextureSwizzle swizzle_b{ TextureSwizzle_B };
			
			TextureSwizzle swizzle_a{ TextureSwizzle_A };
		};

		MAKE_ENUM(TextureSliceType)
		{
			TextureSliceType_2D,
			TextureSliceType_Cube,
			TextureSliceType_3D,
			TextureSliceType_2D_Array,
		};

		NODISCARD virtual RID texture_create(TextureFormat const & format, TextureView const & view, Vector<Vector<byte>> const & data = {}) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* FRAMEBUFFER */

		struct NODISCARD AttachmentFormat final
		{
			DEFAULT_COPYABLE_MOVABLE(AttachmentFormat);

			DataFormat format{ DataFormat_R8G8B8A8_UNORM };
			
			TextureSamples samples{ TextureSamples_1 };
			
			uint32_t usage_flags{ 0 };
		};

		ALIAS(FramebufferFormatID) int64_t;

		virtual RID framebuffer_create(Vector<RID> const & texture_attachments, FramebufferFormatID format_check = -1) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* SAMPLER */

		MAKE_ENUM(SamplerFilter)
		{
			SamplerFilter_Nearest,
			SamplerFilter_Linear,
		};

		MAKE_ENUM(SamplerRepeatMode)
		{
			SamplerRepeatMode_Repeat,
			SamplerRepeatMode_MirroredRepeat,
			SamplerRepeatMode_ClampToEdge,
			SamplerRepeatMode_ClampToBorder,
			SamplerRepeatMode_MirrorClampToEdge,
			SamplerRepeatMode_MAX
		};

		MAKE_ENUM(SamplerBorderColor)
		{
			SamplerBorderColor_Float_Transparent_Black,
			SamplerBorderColor_Int_Transparent_Black,
			SamplerBorderColor_Float_Opaque_Black,
			SamplerBorderColor_Int_Opaque_Black,
			SamplerBorderColor_Float_Opaque_White,
			SamplerBorderColor_Int_Opaque_White,
			SamplerBorderColor_MAX
		};

		struct NODISCARD SamplerState final
		{
			DEFAULT_COPYABLE_MOVABLE(SamplerState);

			SamplerFilter mag_filter{ SamplerFilter_Nearest };
			
			SamplerFilter min_filter{ SamplerFilter_Nearest };
			
			SamplerFilter mip_filter{ SamplerFilter_Nearest };
			
			SamplerRepeatMode repeat_u{ SamplerRepeatMode_ClampToEdge };
			
			SamplerRepeatMode repeat_v{ SamplerRepeatMode_ClampToEdge };
			
			SamplerRepeatMode repeat_w{ SamplerRepeatMode_ClampToEdge };
			
			float_t lod_bias{ 0 };
			
			bool use_anisotropy{ false };
			
			float_t anisotropy_max{ 1.f };
			
			bool enable_compare{};
			
			CompareOperator compare_op{ CompareOperator_Always };
			
			float_t min_lod{ 0 };
			
			float_t max_lod{ 1e20f }; //something very large should do
			
			SamplerBorderColor border_color{ SamplerBorderColor_Float_Opaque_Black };
			
			bool unnormalized_uvw{ false };
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* BUFFER */

		MAKE_ENUM(VertexFrequency)
		{
			VertexFrequency_Vertex,
			VertexFrequency_Instance,
		};

		struct NODISCARD VertexAttribute final
		{
			DEFAULT_COPYABLE_MOVABLE(VertexAttribute);

			uint32_t location{ 0 };
			
			uint32_t offset{ 0 };
			
			uint32_t stride{ 0 };
			
			DataFormat format{ DataFormat_MAX };
			
			VertexFrequency frequency{ VertexFrequency_Vertex };
		};

		ALIAS(VertexFormatID) int64_t;

		MAKE_ENUM(IndexBufferFormat)
		{
			IndexBufferFormat_U16,
			IndexBufferFormat_U32,
		};

		virtual VertexFormatID vertex_format_create(Vector<VertexAttribute> const & vertex_formats) = 0;

		virtual RID vertex_array_create(uint32_t size_bytes, Vector<VertexAttribute> const & vertex_format, Vector<RID> const & src_buffers) = 0;

		virtual RID vertex_buffer_create(uint32_t size_bytes, Vector<byte> const & data = {}, bool use_as_storage = false) = 0;

		virtual RID index_buffer_create(uint32_t size_indices, IndexBufferFormat format, Vector<byte> const & data = {}, bool use_restart_indices = false) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* SHADER */

		MAKE_ENUM(ShaderStage)
		{
			ShaderStage_Vertex,
			ShaderStage_Fragment,
			ShaderStage_TesselationControl,
			ShaderStage_TesselationEvaluation,
			ShaderStage_Compute,
			ShaderStage_MAX,
			ShaderStage_Vertex_Bit					= 1 << ShaderStage_Vertex,
			ShaderStage_Fragment_Bit				= 1 << ShaderStage_Fragment,
			ShaderStage_TesselationControl_Bit		= 1 << ShaderStage_TesselationControl,
			ShaderStage_TesselationEvaluation_Bit	= 1 << ShaderStage_TesselationEvaluation,
			ShaderStage_Compute_Bit					= 1 << ShaderStage_Compute,
		};

		MAKE_ENUM(ShaderLanguage)
		{
			ShaderLanguage_GLSL,
			ShaderLanguage_HLSL,
		};

		struct NODISCARD ShaderStageData final
		{
			DEFAULT_COPYABLE_MOVABLE(ShaderStageData);

			ShaderStage shader_stage{ ShaderStage_Vertex };

			Vector<byte> spir_v{};
		};

		virtual RID shader_create(Vector<ShaderStageData> const & stages) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* UNIFORMS */

		MAKE_ENUM(UniformType)
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

		struct NODISCARD Uniform final
		{
			DEFAULT_COPYABLE_MOVABLE(Uniform);

			UniformType uniform_type{ UniformType_Image };
			
			int32_t binding{ 0 };
			
			Vector<RID> ids{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* RENDER PIPELINE */

		MAKE_ENUM(PolygonCullMode)
		{
			PolygonCullMode_Disabled,
			PolygonCullMode_Front,
			PolygonCullMode_Back,
		};

		MAKE_ENUM(PolygonFrontFace)
		{
			PolygonFrontFace_Clockwise,
			PolygonFrontFace_CounterClockwise,
		};

		MAKE_ENUM(StencilOperation)
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

		MAKE_ENUM(LogicOperation)
		{
			LogicOperation_Clear,
			LogicOperation_And,
			LogicOperation_AndReverse,
			LogicOperation_Copy,
			LogicOperation_AndInverted,
			LogicOperation_NoOp,
			LogicOperation_Xor,
			LogicOperation_Or,
			LogicOperation_Nor,
			LogicOperation_Equivalent,
			LogicOperation_Invert,
			LogicOperation_OrReverse,
			LogicOperation_CopyInverted,
			LogicOperation_OrInverted,
			LogicOperation_Nand,
			LogicOperation_Set,
			LogicOperation_MAX
		};

		MAKE_ENUM(BlendFactor)
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

		MAKE_ENUM(BlendOperation)
		{
			BlendOperation_Add,
			BlendOperation_Subtract,
			BlendOperation_ReverseSubtract,
			BlendOperation_Minimum,
			BlendOperation_Maximum,
			BlendOperation_MAX
		};

		struct NODISCARD PipelineRasterizationState final
		{
			DEFAULT_COPYABLE_MOVABLE(PipelineRasterizationState);

			bool enable_depth_clamp{ false };
			
			bool discard_primitives{ false };
			
			bool wireframe{ false };
			
			PolygonCullMode cull_mode{ PolygonCullMode_Disabled };
			
			PolygonFrontFace front_face{ PolygonFrontFace_Clockwise };
			
			bool depth_bias_enable{ false };
			
			float_t depth_bias_constant_factor{ 0.f };
			
			float_t depth_bias_clamp{ 0.f };
			
			float_t depth_bias_slope_factor{ 0.f };
			
			float_t line_width{ 1.f };
			
			uint32_t patch_control_points{ 1 };
		};

		struct NODISCARD PipelineMultisampleState final
		{
			DEFAULT_COPYABLE_MOVABLE(PipelineMultisampleState);

			TextureSamples sample_count{ TextureSamples_1 };
			
			bool enable_sample_shading{ false };
			
			float_t min_sample_shading{ 0.f };
			
			Vector<uint32_t> sample_mask{};
			
			bool enable_alpha_to_coverage{ false };
			
			bool enable_alpha_to_one{ false };
		};

		struct NODISCARD PipelineDepthStencilState final
		{
			DEFAULT_COPYABLE_MOVABLE(PipelineDepthStencilState);

			struct NODISCARD StencilOperationState final
			{
				DEFAULT_COPYABLE_MOVABLE(StencilOperationState);

				StencilOperation fail{ StencilOperation_Zero };
				
				StencilOperation pass{ StencilOperation_Zero };
				
				StencilOperation depth_fail{ StencilOperation_Zero };
				
				CompareOperator compare{ CompareOperator_Always };
				
				uint32_t compare_mask{ 0 };
				
				uint32_t write_mask{ 0 };
				
				uint32_t reference{ 0 };
			};

			bool enable_depth_test{ false };

			bool enable_depth_write{ false };

			CompareOperator depth_compare_operator{ CompareOperator_Always };

			bool enable_depth_range{ false };

			float_t depth_range_min{ 0.f };

			float_t depth_range_max{ 0.f };

			bool enable_stencil{ false };

			StencilOperationState front_op{};

			StencilOperationState back_op{};
		};

		struct NODISCARD PipelineColorBlendState final
		{
			DEFAULT_COPYABLE_MOVABLE(PipelineColorBlendState);
			
			struct NODISCARD Attachment final
			{
				DEFAULT_COPYABLE_MOVABLE(Attachment);

				bool enable_blend{ false };
				
				BlendFactor src_color_blend_factor{ BlendFactor_Zero };
				
				BlendFactor dst_color_blend_factor{ BlendFactor_Zero };
				
				BlendOperation color_blend_op{ BlendOperation_Add };
				
				BlendFactor src_alpha_blend_factor{ BlendFactor_Zero };
				
				BlendFactor dst_alpha_blend_factor{ BlendFactor_Zero };
				
				BlendOperation alpha_blend_op{ BlendOperation_Add };
				
				bool write_r{ true };
				
				bool write_g{ true };
				
				bool write_b{ true };
				
				bool write_a{ true };
			};

			bool enable_logic_op{ false };

			LogicOperation logic_op{ LogicOperation_Clear };

			Vector<Attachment> attachments{};

			Color blend_constant{};

			NODISCARD static PipelineColorBlendState create_disabled(int32_t num_attachments = 1)
			{
				PipelineColorBlendState bs{};
				bs.attachments.reserve(num_attachments);
				for (int32_t i = 0; i < num_attachments; i++) {
					bs.attachments.push_back(Attachment{});
				}
				return bs;
			}

			NODISCARD static PipelineColorBlendState create_blend(int32_t num_attachments = 1)
			{
				PipelineColorBlendState bs{};
				bs.attachments.reserve(num_attachments);
				for (int32_t i = 0; i < num_attachments; i++) {
					bs.attachments.push_back(Attachment{ true,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add, });
				}
				return bs;
			}
		};

	public: /* SCREEN */

		virtual int32_t screen_get_width(WindowID window = 0) const = 0;
		
		virtual int32_t screen_get_height(WindowID window = 0) const = 0;
		
		virtual FramebufferFormatID screen_get_framebuffer_format() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public: /* MISC */

		virtual void free(RID id) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline RenderingDevice & get_rendering_device() noexcept { return *VALIDATE(RenderingDevice::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
