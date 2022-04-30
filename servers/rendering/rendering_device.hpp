#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <servers/display_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(ColorFormat)
	{
		ColorFormat_R4G4_UNORM_PACK8,
		ColorFormat_R4G4B4A4_UNORM_PACK16,
		ColorFormat_B4G4R4A4_UNORM_PACK16,
		ColorFormat_R5G6B5_UNORM_PACK16,
		ColorFormat_B5G6R5_UNORM_PACK16,
		ColorFormat_R5G5B5A1_UNORM_PACK16,
		ColorFormat_B5G5R5A1_UNORM_PACK16,
		ColorFormat_A1R5G5B5_UNORM_PACK16,
		ColorFormat_R8_UNORM,
		ColorFormat_R8_SNORM,
		ColorFormat_R8_USCALED,
		ColorFormat_R8_SSCALED,
		ColorFormat_R8_UINT,
		ColorFormat_R8_SINT,
		ColorFormat_R8_SRGB,
		ColorFormat_R8G8_UNORM,
		ColorFormat_R8G8_SNORM,
		ColorFormat_R8G8_USCALED,
		ColorFormat_R8G8_SSCALED,
		ColorFormat_R8G8_UINT,
		ColorFormat_R8G8_SINT,
		ColorFormat_R8G8_SRGB,
		ColorFormat_R8G8B8_UNORM,
		ColorFormat_R8G8B8_SNORM,
		ColorFormat_R8G8B8_USCALED,
		ColorFormat_R8G8B8_SSCALED,
		ColorFormat_R8G8B8_UINT,
		ColorFormat_R8G8B8_SINT,
		ColorFormat_R8G8B8_SRGB,
		ColorFormat_B8G8R8_UNORM,
		ColorFormat_B8G8R8_SNORM,
		ColorFormat_B8G8R8_USCALED,
		ColorFormat_B8G8R8_SSCALED,
		ColorFormat_B8G8R8_UINT,
		ColorFormat_B8G8R8_SINT,
		ColorFormat_B8G8R8_SRGB,
		ColorFormat_R8G8B8A8_UNORM,
		ColorFormat_R8G8B8A8_SNORM,
		ColorFormat_R8G8B8A8_USCALED,
		ColorFormat_R8G8B8A8_SSCALED,
		ColorFormat_R8G8B8A8_UINT,
		ColorFormat_R8G8B8A8_SINT,
		ColorFormat_R8G8B8A8_SRGB,
		ColorFormat_B8G8R8A8_UNORM,
		ColorFormat_B8G8R8A8_SNORM,
		ColorFormat_B8G8R8A8_USCALED,
		ColorFormat_B8G8R8A8_SSCALED,
		ColorFormat_B8G8R8A8_UINT,
		ColorFormat_B8G8R8A8_SINT,
		ColorFormat_B8G8R8A8_SRGB,
		ColorFormat_A8B8G8R8_UNORM_PACK32,
		ColorFormat_A8B8G8R8_SNORM_PACK32,
		ColorFormat_A8B8G8R8_USCALED_PACK32,
		ColorFormat_A8B8G8R8_SSCALED_PACK32,
		ColorFormat_A8B8G8R8_UINT_PACK32,
		ColorFormat_A8B8G8R8_SINT_PACK32,
		ColorFormat_A8B8G8R8_SRGB_PACK32,
		ColorFormat_A2R10G10B10_UNORM_PACK32,
		ColorFormat_A2R10G10B10_SNORM_PACK32,
		ColorFormat_A2R10G10B10_USCALED_PACK32,
		ColorFormat_A2R10G10B10_SSCALED_PACK32,
		ColorFormat_A2R10G10B10_UINT_PACK32,
		ColorFormat_A2R10G10B10_SINT_PACK32,
		ColorFormat_A2B10G10R10_UNORM_PACK32,
		ColorFormat_A2B10G10R10_SNORM_PACK32,
		ColorFormat_A2B10G10R10_USCALED_PACK32,
		ColorFormat_A2B10G10R10_SSCALED_PACK32,
		ColorFormat_A2B10G10R10_UINT_PACK32,
		ColorFormat_A2B10G10R10_SINT_PACK32,
		ColorFormat_R16_UNORM,
		ColorFormat_R16_SNORM,
		ColorFormat_R16_USCALED,
		ColorFormat_R16_SSCALED,
		ColorFormat_R16_UINT,
		ColorFormat_R16_SINT,
		ColorFormat_R16_SFLOAT,
		ColorFormat_R16G16_UNORM,
		ColorFormat_R16G16_SNORM,
		ColorFormat_R16G16_USCALED,
		ColorFormat_R16G16_SSCALED,
		ColorFormat_R16G16_UINT,
		ColorFormat_R16G16_SINT,
		ColorFormat_R16G16_SFLOAT,
		ColorFormat_R16G16B16_UNORM,
		ColorFormat_R16G16B16_SNORM,
		ColorFormat_R16G16B16_USCALED,
		ColorFormat_R16G16B16_SSCALED,
		ColorFormat_R16G16B16_UINT,
		ColorFormat_R16G16B16_SINT,
		ColorFormat_R16G16B16_SFLOAT,
		ColorFormat_R16G16B16A16_UNORM,
		ColorFormat_R16G16B16A16_SNORM,
		ColorFormat_R16G16B16A16_USCALED,
		ColorFormat_R16G16B16A16_SSCALED,
		ColorFormat_R16G16B16A16_UINT,
		ColorFormat_R16G16B16A16_SINT,
		ColorFormat_R16G16B16A16_SFLOAT,
		ColorFormat_R32_UINT,
		ColorFormat_R32_SINT,
		ColorFormat_R32_SFLOAT,
		ColorFormat_R32G32_UINT,
		ColorFormat_R32G32_SINT,
		ColorFormat_R32G32_SFLOAT,
		ColorFormat_R32G32B32_UINT,
		ColorFormat_R32G32B32_SINT,
		ColorFormat_R32G32B32_SFLOAT,
		ColorFormat_R32G32B32A32_UINT,
		ColorFormat_R32G32B32A32_SINT,
		ColorFormat_R32G32B32A32_SFLOAT,
		ColorFormat_R64_UINT,
		ColorFormat_R64_SINT,
		ColorFormat_R64_SFLOAT,
		ColorFormat_R64G64_UINT,
		ColorFormat_R64G64_SINT,
		ColorFormat_R64G64_SFLOAT,
		ColorFormat_R64G64B64_UINT,
		ColorFormat_R64G64B64_SINT,
		ColorFormat_R64G64B64_SFLOAT,
		ColorFormat_R64G64B64A64_UINT,
		ColorFormat_R64G64B64A64_SINT,
		ColorFormat_R64G64B64A64_SFLOAT,
		ColorFormat_B10G11R11_UFLOAT_PACK32,
		ColorFormat_E5B9G9R9_UFLOAT_PACK32,
		ColorFormat_D16_UNORM,
		ColorFormat_X8_D24_UNORM_PACK32,
		ColorFormat_D32_SFLOAT,
		ColorFormat_S8_UINT,
		ColorFormat_D16_UNORM_S8_UINT,
		ColorFormat_D24_UNORM_S8_UINT,
		ColorFormat_D32_SFLOAT_S8_UINT,
		ColorFormat_BC1_RGB_UNORM_BLOCK,
		ColorFormat_BC1_RGB_SRGB_BLOCK,
		ColorFormat_BC1_RGBA_UNORM_BLOCK,
		ColorFormat_BC1_RGBA_SRGB_BLOCK,
		ColorFormat_BC2_UNORM_BLOCK,
		ColorFormat_BC2_SRGB_BLOCK,
		ColorFormat_BC3_UNORM_BLOCK,
		ColorFormat_BC3_SRGB_BLOCK,
		ColorFormat_BC4_UNORM_BLOCK,
		ColorFormat_BC4_SNORM_BLOCK,
		ColorFormat_BC5_UNORM_BLOCK,
		ColorFormat_BC5_SNORM_BLOCK,
		ColorFormat_BC6H_UFLOAT_BLOCK,
		ColorFormat_BC6H_SFLOAT_BLOCK,
		ColorFormat_BC7_UNORM_BLOCK,
		ColorFormat_BC7_SRGB_BLOCK,
		ColorFormat_ETC2_R8G8B8_UNORM_BLOCK,
		ColorFormat_ETC2_R8G8B8_SRGB_BLOCK,
		ColorFormat_ETC2_R8G8B8A1_UNORM_BLOCK,
		ColorFormat_ETC2_R8G8B8A1_SRGB_BLOCK,
		ColorFormat_ETC2_R8G8B8A8_UNORM_BLOCK,
		ColorFormat_ETC2_R8G8B8A8_SRGB_BLOCK,
		ColorFormat_EAC_R11_UNORM_BLOCK,
		ColorFormat_EAC_R11_SNORM_BLOCK,
		ColorFormat_EAC_R11G11_UNORM_BLOCK,
		ColorFormat_EAC_R11G11_SNORM_BLOCK,
		ColorFormat_ASTC_4x4_UNORM_BLOCK,
		ColorFormat_ASTC_4x4_SRGB_BLOCK,
		ColorFormat_ASTC_5x4_UNORM_BLOCK,
		ColorFormat_ASTC_5x4_SRGB_BLOCK,
		ColorFormat_ASTC_5x5_UNORM_BLOCK,
		ColorFormat_ASTC_5x5_SRGB_BLOCK,
		ColorFormat_ASTC_6x5_UNORM_BLOCK,
		ColorFormat_ASTC_6x5_SRGB_BLOCK,
		ColorFormat_ASTC_6x6_UNORM_BLOCK,
		ColorFormat_ASTC_6x6_SRGB_BLOCK,
		ColorFormat_ASTC_8x5_UNORM_BLOCK,
		ColorFormat_ASTC_8x5_SRGB_BLOCK,
		ColorFormat_ASTC_8x6_UNORM_BLOCK,
		ColorFormat_ASTC_8x6_SRGB_BLOCK,
		ColorFormat_ASTC_8x8_UNORM_BLOCK,
		ColorFormat_ASTC_8x8_SRGB_BLOCK,
		ColorFormat_ASTC_10x5_UNORM_BLOCK,
		ColorFormat_ASTC_10x5_SRGB_BLOCK,
		ColorFormat_ASTC_10x6_UNORM_BLOCK,
		ColorFormat_ASTC_10x6_SRGB_BLOCK,
		ColorFormat_ASTC_10x8_UNORM_BLOCK,
		ColorFormat_ASTC_10x8_SRGB_BLOCK,
		ColorFormat_ASTC_10x10_UNORM_BLOCK,
		ColorFormat_ASTC_10x10_SRGB_BLOCK,
		ColorFormat_ASTC_12x10_UNORM_BLOCK,
		ColorFormat_ASTC_12x10_SRGB_BLOCK,
		ColorFormat_ASTC_12x12_UNORM_BLOCK,
		ColorFormat_ASTC_12x12_SRGB_BLOCK,
		ColorFormat_G8B8G8R8_422_UNORM,
		ColorFormat_B8G8R8G8_422_UNORM,
		ColorFormat_G8_B8_R8_3PLANE_420_UNORM,
		ColorFormat_G8_B8R8_2PLANE_420_UNORM,
		ColorFormat_G8_B8_R8_3PLANE_422_UNORM,
		ColorFormat_G8_B8R8_2PLANE_422_UNORM,
		ColorFormat_G8_B8_R8_3PLANE_444_UNORM,
		ColorFormat_R10X6_UNORM_PACK16,
		ColorFormat_R10X6G10X6_UNORM_2PACK16,
		ColorFormat_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
		ColorFormat_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
		ColorFormat_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
		ColorFormat_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
		ColorFormat_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
		ColorFormat_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
		ColorFormat_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
		ColorFormat_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
		ColorFormat_R12X4_UNORM_PACK16,
		ColorFormat_R12X4G12X4_UNORM_2PACK16,
		ColorFormat_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
		ColorFormat_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
		ColorFormat_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
		ColorFormat_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
		ColorFormat_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
		ColorFormat_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
		ColorFormat_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
		ColorFormat_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
		ColorFormat_G16B16G16R16_422_UNORM,
		ColorFormat_B16G16R16G16_422_UNORM,
		ColorFormat_G16_B16_R16_3PLANE_420_UNORM,
		ColorFormat_G16_B16R16_2PLANE_420_UNORM,
		ColorFormat_G16_B16_R16_3PLANE_422_UNORM,
		ColorFormat_G16_B16R16_2PLANE_422_UNORM,
		ColorFormat_G16_B16_R16_3PLANE_444_UNORM,
		ColorFormat_PVRTC1_2BPP_UNORM_BLOCK_IMG,
		ColorFormat_PVRTC1_4BPP_UNORM_BLOCK_IMG,
		ColorFormat_PVRTC2_2BPP_UNORM_BLOCK_IMG,
		ColorFormat_PVRTC2_4BPP_UNORM_BLOCK_IMG,
		ColorFormat_PVRTC1_2BPP_SRGB_BLOCK_IMG,
		ColorFormat_PVRTC1_4BPP_SRGB_BLOCK_IMG,
		ColorFormat_PVRTC2_2BPP_SRGB_BLOCK_IMG,
		ColorFormat_PVRTC2_4BPP_SRGB_BLOCK_IMG,
		ColorFormat_MAX
	};

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

	ENUM_INT(FramebufferTextureType)
	{
		FramebufferTextureType_Color,
		FramebufferTextureType_RedInteger,
		FramebufferTextureType_DepthStencil,
	};

	ALIAS(FramebufferAttachmentSpecification) Vector<FramebufferTextureType_>;

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

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD TextureSpecification
	{
		DEFAULT_COPYABLE_MOVABLE(TextureSpecification);
		
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

	struct NODISCARD ShaderStageData
	{
		DEFAULT_COPYABLE_MOVABLE(ShaderStageData);
		
		ShaderStage_ shader_stage{ ShaderStage_Vertex };
		
		Buffer source{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD FramebufferSpecification
	{
		DEFAULT_COPYABLE_MOVABLE(FramebufferSpecification);

		uint32_t width{ 1280 }, height{ 720 };

		FramebufferAttachmentSpecification attachments{ FramebufferTextureType_Color, FramebufferTextureType_DepthStencil };

		bool swap_chain_target{ false };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD Uniform
	{
		DEFAULT_COPYABLE_MOVABLE(Uniform);
		
		UniformType_ uniform_type{ UniformType_Image };
		
		int32_t binding{ 0 };
		
		Vector<RID> ids{};
	};

	struct NODISCARD SamplerState
	{
		DEFAULT_COPYABLE_MOVABLE(SamplerState);
		
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD PipelineRasterizationState
	{
		DEFAULT_COPYABLE_MOVABLE(PipelineRasterizationState);
		
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

	struct NODISCARD PipelineMultisampleState
	{
		DEFAULT_COPYABLE_MOVABLE(PipelineMultisampleState);
		
		TextureSamples_ sample_count{ TextureSamples_1 };
		
		bool enable_sample_shading{ false };
		
		float_t min_sample_shading{ 0.f };
		
		Buffer sample_mask{};
		
		bool enable_alpha_to_coverage{ false };
		
		bool enable_alpha_to_one{ false };
	};

	struct NODISCARD PipelineDepthStencilState
	{
		struct NODISCARD StencilOperationState
		{
			DEFAULT_COPYABLE_MOVABLE(StencilOperationState);
			
			StencilOperation_ fail{ StencilOperation_Zero };
			
			StencilOperation_ pass{ StencilOperation_Zero };
			
			StencilOperation_ depth_fail{ StencilOperation_Zero };
			
			CompareOperator_ compare{ CompareOperator_Always };
			
			uint32_t compare_mask{ 0 };
			
			uint32_t write_mask{ 0 };
			
			uint32_t reference{ 0 };
		};

		DEFAULT_COPYABLE_MOVABLE(PipelineDepthStencilState);
		
		bool enable_depth_test{ false };
		
		bool enable_depth_write{ false };
		
		CompareOperator_ depth_compare_operator{ CompareOperator_Always };
		
		bool enable_depth_range{ false };
		
		float_t depth_range_min{ 0.f }, depth_range_max{ 0.f };
		
		bool enable_stencil{ false };
		
		StencilOperationState front_op{}, back_op{};
	};

	struct NODISCARD PipelineColorBlendState
	{
		struct NODISCARD Attachment
		{
			DEFAULT_COPYABLE_MOVABLE(Attachment);
			
			bool enable_blend{ false };
			
			BlendFactor_ src_color_blend_factor{ BlendFactor_Zero };
			
			BlendFactor_ dst_color_blend_factor{ BlendFactor_Zero };
			
			BlendOperation_ color_blend_op{ BlendOperation_Add };
			
			BlendFactor_ src_alpha_blend_factor{ BlendFactor_Zero };
			
			BlendFactor_ dst_alpha_blend_factor{ BlendFactor_Zero };
			
			BlendOperation_ alpha_blend_op{ BlendOperation_Add };
			
			bool write_r{ true }, write_g{ true }, write_b{ true }, write_a{ true };
		};

		DEFAULT_COPYABLE_MOVABLE(PipelineColorBlendState);
		bool enable_logic_op{ false };
		LogicOperation_ logic_op{ LogicOperation_Clear };
		Vector<Attachment> attachments{};
		Color blend_constant{};

		NODISCARD static PipelineColorBlendState create_disabled(int32_t num_attachments = 1)
		{
			PipelineColorBlendState temp{};
			temp.attachments.reserve(num_attachments);
			for (int32_t i = 0; i < num_attachments; i++) {
				temp.attachments.push_back(Attachment{});
			}
			return temp;
		}

		NODISCARD static PipelineColorBlendState create_blend(int32_t num_attachments = 1)
		{
			PipelineColorBlendState temp{};
			temp.attachments.reserve(num_attachments);
			for (int32_t i = 0; i < num_attachments; i++) {
				temp.attachments.push_back(Attachment{ true,
					BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
					BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add, });
			}
			return temp;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD VertexLayout
	{
		struct NODISCARD Element
		{
			CONSTEXPR_DEFAULT_COPYABLE_MOVABLE(Element);

			cstring		name		{};
			DataType_	type		{};
			uint32_t	count		{};
			bool		normalized	{};
			uint32_t	size		{ ism::get_data_type_size(type) * count };
			uint32_t	offset		{};
		};

		uint32_t stride{};

		Vector<Element> elements{};

		template <class It> VertexLayout(It first, It last) noexcept : elements{ first, last }
		{
			uint32_t offset{};
			for (Element & e : elements)
			{
				e.offset = offset;
				offset += e.size;
				stride += e.size;
			}
		}

		VertexLayout(std::initializer_list<Element> init) noexcept : VertexLayout{ init.begin(), init.end() } {}

		template <size_t N> VertexLayout(Element const (&arr)[N]) noexcept : VertexLayout{ &arr[0], &arr[N] } {}

		VertexLayout() noexcept : VertexLayout{
			{ "a_Position",	DataType_F32, 3 },
			{ "a_Normal",	DataType_F32, 3 },
			{ "a_Texcoord",	DataType_F32, 2 }, } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

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
		virtual RID vertexarray_create(VertexLayout const & layout, RID indices, RID vertices) = 0;
		virtual void vertexarray_destroy(RID rid) = 0;
		virtual void vertexarray_bind(RID rid) = 0;
		virtual void vertexarray_draw(RID rid) = 0;
		
	public:
		/* VERTEXBUFFER */
		virtual RID vertexbuffer_create(Buffer const & data) = 0;
		virtual void vertexbuffer_destroy(RID rid) = 0;
		virtual void vertexbuffer_bind(RID rid) = 0;
		virtual void vertexbuffer_update(RID rid, Buffer const & data, size_t offset = 0) = 0;
		
	public:
		/* INDEXBUFFER */
		virtual RID indexbuffer_create(Buffer const & data) = 0;
		virtual void indexbuffer_destroy(RID rid) = 0;
		virtual void indexbuffer_bind(RID rid) = 0;
		virtual void indexbuffer_update(RID rid, Buffer const & data, size_t offset = 0) = 0;

	public:
		/* TEXTURE */
		virtual RID texture_create(TextureSpecification const & format, Buffer const & data = {}) = 0;
		virtual void texture_destroy(RID rid) = 0;
		virtual void texture_bind(RID rid, size_t slot = 0) = 0;
		virtual void texture_update(RID rid, Buffer const & data = {}) = 0;
		virtual Buffer texture_get_data(RID rid) = 0;

	public:
		/* FRAMEBUFFER */
		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) = 0;
		virtual void framebuffer_destroy(RID rid) = 0;
		virtual void framebuffer_bind(RID rid) = 0;
		virtual void framebuffer_update(RID rid, int32_t width, int32_t height) = 0;

	public:
		/* SHADER */
		virtual RID shader_create(Vector<ShaderStageData> const & stage_data) = 0;
		virtual void shader_destroy(RID rid) = 0;
		virtual void shader_bind(RID rid) = 0;
		virtual int32_t shader_uniform_location(RID rid, cstring name) const = 0;
		virtual void shader_set_uniform1i(RID rid, cstring name, int32_t const value) = 0;
		virtual void shader_set_uniform1f(RID rid, cstring name, float_t const value) = 0;
		virtual void shader_set_uniform2f(RID rid, cstring name, Vec2 const & value) = 0;
		virtual void shader_set_uniform3f(RID rid, cstring name, Vec3 const & value) = 0;
		virtual void shader_set_uniform4f(RID rid, cstring name, Vec4 const & value) = 0;
		virtual void shader_set_uniform16f(RID rid, cstring name, Mat4 const & value, bool transpose = false) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// rendering device singleton
#define RD (ism::RenderingDevice::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
