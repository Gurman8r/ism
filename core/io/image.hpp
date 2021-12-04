#ifndef _ISM_IMAGE_HPP_
#define _ISM_IMAGE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
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

	class ISM_API Image : public Resource
	{
		OBJECT_COMMON(Image, Resource);

		friend class ImageLoader;

		Vector<byte> m_pixels{};

		int32_t m_width{}, m_height{}, m_channels{};

		ImageFormat_ m_format{};

		bool m_has_mipmaps{};

	public:
		Image() noexcept {}
		virtual ~Image() noexcept override = default;

		void flip_vertically();
		void flip_horizontally();

		NODISCARD Color32 get_pixel(size_t i) const;
		NODISCARD Color32 get_pixel(size_t x, size_t y) const;
		void set_pixel(size_t i, Color32 value);
		void set_pixel(size_t x, size_t y, Color32 value);

		NODISCARD int32_t get_width() const noexcept { return m_width; }
		NODISCARD int32_t get_height() const noexcept { return m_height; }
		NODISCARD int32_t get_channels() const noexcept { return m_channels; }
		NODISCARD ImageFormat_ get_format() const noexcept { return m_format; }
		NODISCARD Vector<byte> const & get_data() const noexcept { return m_pixels; }
		NODISCARD bool has_mipmaps() const noexcept { return m_has_mipmaps; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_HPP_
