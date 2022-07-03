#ifndef _ISM_IMAGE_HPP_
#define _ISM_IMAGE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Image : public Resource
	{
		OBJECT_COMMON(Image, Resource);

		friend class ImageLoader;

	public:
		enum Format_
		{
			Format_L8, //luminance
			Format_LA8, //luminance-alpha
			Format_R8,
			Format_RG8,
			Format_RGB8,
			Format_RGBA8,
			Format_RGBA4444,
			Format_RGB565,
			Format_RF, //float
			Format_RGF,
			Format_RGBF,
			Format_RGBAF,
			Format_RH, //half float
			Format_RGH,
			Format_RGBH,
			Format_RGBAH,
			Format_RGBE9995,
			Format_DXT1, //s3tc bc1
			Format_DXT3, //bc2
			Format_DXT5, //bc3
			Format_RGTC_R,
			Format_RGTC_RG,
			Format_BPTC_RGBA, //btpc bc7
			Format_BPTC_RGBF, //float bc6h
			Format_BPTC_RGBFU, //unsigned float bc6hu
			Format_PVRTC1_2, //pvrtc1
			Format_PVRTC1_2A,
			Format_PVRTC1_4,
			Format_PVRTC1_4A,
			Format_ETC, //etc1
			Format_ETC2_R11, //etc2
			Format_ETC2_R11S, //signed, NOT srgb.
			Format_ETC2_RG11,
			Format_ETC2_RG11S,
			Format_ETC2_RGB8,
			Format_ETC2_RGBA8,
			Format_ETC2_RGB8A1,
			Format_ETC2_RA_AS_RG, //used to make basis universal happy
			Format_DXT5_RA_AS_RG, //used to make basis universal happy
			Format_MAX
		};

		enum Interpolation_
		{
			Interpolation_Nearest,
			Interpolation_Bilinear,
			Interpolation_Cubic,
			Interpolation_Trilinear,
			Interpolation_Lanczos,
		};

	private:
		Format_ m_format{};

		int32_t m_width{}, m_height{}, m_depth{};
		
		DynamicBuffer m_pixels{};

	public:
		using iterator = typename decltype(m_pixels)::iterator;
		using const_iterator = typename decltype(m_pixels)::const_iterator;

	public:
		Image() noexcept {}

		explicit Image(Path const & path) noexcept { set_path(path); reload_from_file(); }

		Image(int32_t width, int32_t height, Format_ format);

		Image(int32_t width, int32_t height, Format_ format, DynamicBuffer const & data);

		virtual Error_ reload_from_file() override;

		void clear();

		void flip_vertically();

		void flip_horizontally();

	public:
		virtual RID get_rid() const override { return RID{}; }

		int32_t get_width() const noexcept { return m_width; }

		int32_t get_height() const noexcept { return m_height; }

		int32_t get_depth() const noexcept { return m_depth; }

		int32_t get_capacity() const noexcept { return m_width * m_height * m_depth; }

		Format_ get_format() const noexcept { return m_format; }

		DynamicBuffer const & get_data() const noexcept { return m_pixels; }

		Color32 get_pixel(size_t index) const;

		Color32 get_pixel(size_t x, size_t y) const noexcept { return get_pixel((x + y * m_width) * m_depth); }

		void set_pixel(size_t index, Color32 value);

		void set_pixel(size_t x, size_t y, Color32 value) noexcept { set_pixel(((x + y * m_width) * m_depth), value); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr size_t get_image_depth(Image::Format_ image_format) noexcept
	{
		switch (image_format) {
		case Image::Format_R8: return 1;
		case Image::Format_RG8: return 2;
		case Image::Format_RGB8: return 3;
		case Image::Format_RGBA8: return 4;
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_HPP_
