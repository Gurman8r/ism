#ifndef _ISM_IMAGE_HPP_
#define _ISM_IMAGE_HPP_

#include <core/io/resource.hpp>

// enums
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

	constexpr bool is_depth_stencil_color_format(ColorFormat_ value) noexcept
	{
		switch (value)
		{
		case ColorFormat_D16_UNORM:
		case ColorFormat_X8_D24_UNORM_PACK32:
		case ColorFormat_D32_SFLOAT:
		case ColorFormat_D16_UNORM_S8_UINT:
		case ColorFormat_D24_UNORM_S8_UINT:
		case ColorFormat_D32_SFLOAT_S8_UINT:
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(ColorChannel)
	{
		ColorChannel_L,
		ColorChannel_LA,
		ColorChannel_R,
		ColorChannel_RG,
		ColorChannel_RGB,
		ColorChannel_RGBA,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(ImageFormat)
	{
		ImageFormat_L8, //luminance
		ImageFormat_LA8, //luminance-alpha
		ImageFormat_R8,
		ImageFormat_RG8,
		ImageFormat_RGB8,
		ImageFormat_RGBA8,
		ImageFormat_RGBA4444,
		ImageFormat_RGB565,
		ImageFormat_RF, //float
		ImageFormat_RGF,
		ImageFormat_RGBF,
		ImageFormat_RGBAF,
		ImageFormat_RH, //half float
		ImageFormat_RGH,
		ImageFormat_RGBH,
		ImageFormat_RGBAH,
		ImageFormat_RGBE9995,
		ImageFormat_DXT1, //s3tc bc1
		ImageFormat_DXT3, //bc2
		ImageFormat_DXT5, //bc3
		ImageFormat_RGTC_R,
		ImageFormat_RGTC_RG,
		ImageFormat_BPTC_RGBA, //btpc bc7
		ImageFormat_BPTC_RGBF, //float bc6h
		ImageFormat_BPTC_RGBFU, //unsigned float bc6hu
		ImageFormat_PVRTC1_2, //pvrtc1
		ImageFormat_PVRTC1_2A,
		ImageFormat_PVRTC1_4,
		ImageFormat_PVRTC1_4A,
		ImageFormat_ETC, //etc1
		ImageFormat_ETC2_R11, //etc2
		ImageFormat_ETC2_R11S, //signed, NOT srgb.
		ImageFormat_ETC2_RG11,
		ImageFormat_ETC2_RG11S,
		ImageFormat_ETC2_RGB8,
		ImageFormat_ETC2_RGBA8,
		ImageFormat_ETC2_RGB8A1,
		ImageFormat_ETC2_RA_AS_RG, //used to make basis universal happy
		ImageFormat_DXT5_RA_AS_RG, //used to make basis universal happy
		ImageFormat_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ENUM_INT(Interpolation)
	{
		Interpolation_Nearest,
		Interpolation_Bilinear,
		Interpolation_Cubic,
		Interpolation_Trilinear,
		Interpolation_Lanczos,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// image
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Image : public Resource
	{
		OBJECT_COMMON(Image, Resource);

		friend class ImageLoader;

		Buffer m_pixels{};

		int32_t m_width{}, m_height{}, m_channels{};

		ImageFormat_ m_format{};

	public:
		virtual ~Image() noexcept override = default;
		
		Image() noexcept {}
		
		Image(int32_t width, int32_t height, ImageFormat_ format);
		
		Image(int32_t width, int32_t height, ImageFormat_ format, Buffer const & data);

		void flip_vertically();
		
		void flip_horizontally();

		NODISCARD Color32 get_pixel(size_t i) const;
		
		NODISCARD Color32 get_pixel(size_t x, size_t y) const;
		
		void set_pixel(size_t i, Color32 value);
		
		void set_pixel(size_t x, size_t y, Color32 value);

		NODISCARD auto get_width() const noexcept -> int32_t { return m_width; }
		
		NODISCARD auto get_height() const noexcept -> int32_t { return m_height; }
		
		NODISCARD auto get_channel_count() const noexcept -> int32_t { return m_channels; }
		
		NODISCARD auto get_format() const noexcept -> ImageFormat_ { return m_format; }
		
		NODISCARD auto get_data() const noexcept -> Buffer const & { return m_pixels; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_HPP_
