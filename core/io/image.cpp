#include <core/io/image.hpp>
#include <core/io/image_loader.hpp>

using namespace ism;

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
	return ImageLoader::load_image(*this, get_path());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

Color32 Image::get_pixel(size_t i) const
{
	const_iterator it{ m_pixels.begin() + i };
	byte const r{ m_depth >= 1 ? *(it + i + 0) : (byte)0 };
	byte const g{ m_depth >= 2 ? *(it + i + 1) : (byte)0 };
	byte const b{ m_depth >= 3 ? *(it + i + 2) : (byte)0 };
	byte const a{ m_depth >= 4 ? *(it + i + 3) : (byte)0xFF };
	return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | ((uint32_t)r << 0);
}

Color32 Image::get_pixel(size_t x, size_t y) const
{
	return get_pixel((x + y * m_width) * m_depth);
}

void Image::set_pixel(size_t i, Color32 value)
{
	iterator it{ m_pixels.begin() + i };
	if (m_depth >= 1) { *it = (byte)((int32_t)(value >> 0) & 0xFF); }
	if (m_depth >= 2) { *it = (byte)((int32_t)(value >> 8) & 0xFF); }
	if (m_depth >= 3) { *it = (byte)((int32_t)(value >> 16) & 0xFF); }
	if (m_depth >= 4) { *it = (byte)((int32_t)(value >> 24) & 0xFF); }
}

void Image::set_pixel(size_t x, size_t y, Color32 value)
{
	set_pixel(((x + y * m_width) * m_depth), value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */