#include <core/io/image_loader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Image, t) {}

	Image::Image(int32_t width, int32_t height, Format_ format)
	{
		m_width = width;
		m_height = height;
		m_format = format;
		switch (format) {
		default: { CRASH("UNSUPPORTED IMAGE FORMAT"); } break;
		case Image::Format_R8: { m_depth = 1; } break;
		case Image::Format_RG8: { m_depth = 2; } break;
		case Image::Format_RGB8: { m_depth = 3; } break;
		case Image::Format_RGBA8: { m_depth = 4; } break;
		}
		m_pixels.resize((size_t)(m_width * m_height * m_depth));
	}

	Image::Image(int32_t width, int32_t height, Format_ format, DynamicBuffer const & data)
	{
		m_width = width;
		m_height = height;
		m_format = format;
		m_pixels = data;
		switch (format) {
		default: { CRASH("UNSUPPORTED IMAGE FORMAT"); } break;
		case Image::Format_R8: { m_depth = 1; } break;
		case Image::Format_RG8: { m_depth = 2; } break;
		case Image::Format_RGB8: { m_depth = 3; } break;
		case Image::Format_RGBA8: { m_depth = 4; } break;
		}
	}

	Error_ Image::reload_from_file()
	{
		return ImageLoader::load_from_file(*this, get_path());
	}

	void Image::clear()
	{
		m_pixels = {};
		m_width = m_height = m_depth = 0;
		m_format = Image::Format_MAX;
	}

	void Image::flip_vertically()
	{
		ptrdiff_t const columns{ m_width * m_depth };

		for (ptrdiff_t y = 0; y < m_height; ++y)
		{
			iterator left{ m_pixels.begin() + y * columns };

			iterator right{ m_pixels.begin() + (y + 1) * columns - m_depth };

			for (ptrdiff_t x = 0; x < m_width / 2; ++x)
			{
				std::swap_ranges(left, left + m_depth, right);
				left += m_depth;
				right -= m_depth;
			}
		}
	}

	void Image::flip_horizontally()
	{
		ptrdiff_t const columns{ m_width * m_depth };

		iterator top{ m_pixels.begin() };

		iterator bottom{ m_pixels.end() - columns };

		for (ptrdiff_t y = 0; y < m_height / 2; ++y)
		{
			std::swap_ranges(top, top + columns, bottom);
			top += columns;
			bottom -= columns;
		}
	}

	Color32 Image::get_pixel(size_t index) const
	{
		ASSERT((int32_t)index + m_depth < get_capacity());
		const_iterator const it{ m_pixels.begin() + index };
		byte r{}, g{}, b{}, a{ byte::max };
		if (m_depth >= 1) { r = *(it + 0); }
		if (m_depth >= 2) { g = *(it + 1); }
		if (m_depth >= 3) { b = *(it + 2); }
		if (m_depth >= 4) { a = *(it + 3); }
		return COLOR32(r, g, b, a);
	}

	void Image::set_pixel(size_t index, Color32 value)
	{
		ASSERT((int32_t)index + m_depth < get_capacity());
		iterator const it{ m_pixels.begin() + index };
		if (m_depth >= 1) { *(it + 0) = COLOR32_R(value); }
		if (m_depth >= 2) { *(it + 1) = COLOR32_G(value); }
		if (m_depth >= 3) { *(it + 2) = COLOR32_B(value); }
		if (m_depth >= 4) { *(it + 3) = COLOR32_A(value); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}