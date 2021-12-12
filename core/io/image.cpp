#include <core/io/image_loader.hpp>

using namespace ism;

EMBED_CLASS(Image, t)
{
	t.tp_bind = CLASS_BINDER(Image, t)
	{
		return t
			.def(init<>())
			.def(init([]() { return Image{}; }))
			;
	};
}

Image::Image(int32_t width, int32_t height, ImageFormat_ format)
{
	m_width = width;
	m_height = height;
	m_format = format;
	switch (format)
	{
	default: { CRASH("Unsupported image format"); } break;
	case ImageFormat_R8: { m_channels = 1; } break;
	case ImageFormat_RG8: { m_channels = 2; } break;
	case ImageFormat_RGB8: { m_channels = 3; } break;
	case ImageFormat_RGBA8: { m_channels = 4; } break;
	}
	m_pixels.resize((size_t)(m_width * m_height * m_channels));
}

Image::Image(int32_t width, int32_t height, ImageFormat_ format, Buffer const & data)
{
	m_width = width;
	m_height = height;
	m_format = format;
	m_pixels = data;
	switch (format)
	{
	default: { CRASH("Unsupported image format"); } break;
	case ImageFormat_R8: { m_channels = 1; } break;
	case ImageFormat_RG8: { m_channels = 2; } break;
	case ImageFormat_RGB8: { m_channels = 3; } break;
	case ImageFormat_RGBA8: { m_channels = 4; } break;
	}
}

void Image::flip_vertically()
{
	ptrdiff_t const columns{ m_width * m_channels };

	for (ptrdiff_t y = 0; y < m_height; ++y)
	{
		auto lhs{ m_pixels.begin() + y * columns };

		auto rhs{ m_pixels.begin() + (y + 1) * columns - m_channels };

		for (ptrdiff_t x = 0; x < m_width / 2; ++x)
		{
			std::swap_ranges(lhs, lhs + m_channels, rhs);
			lhs += m_channels;
			rhs -= m_channels;
		}
	}
}

void Image::flip_horizontally()
{
	ptrdiff_t const columns{ m_width * m_channels };

	auto top{ m_pixels.begin() };

	auto bot{ m_pixels.end() - columns };

	for (ptrdiff_t y = 0; y < m_height / 2; ++y)
	{
		std::swap_ranges(top, top + columns, bot);
		top += columns;
		bot -= columns;
	}
}

Color32 Image::get_pixel(size_t i) const
{
	auto it{ m_pixels.begin() + i };
	byte r = m_channels >= 1 ? *(it + i + 0) : byte{};
	byte g = m_channels >= 2 ? *(it + i + 1) : byte{};
	byte b = m_channels >= 3 ? *(it + i + 2) : byte{};
	byte a = m_channels >= 4 ? *(it + i + 3) : (byte)0xFF;
	return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | ((uint32_t)r << 0);
}

Color32 Image::get_pixel(size_t x, size_t y) const
{
	return get_pixel((x + y * m_width) * m_channels);
}

void Image::set_pixel(size_t i, Color32 value)
{
	auto it{ m_pixels.begin() + i };
	if (m_channels >= 1) { *it = (byte)((int32_t)(value >> 0) & 0xFF); }
	if (m_channels >= 2) { *it = (byte)((int32_t)(value >> 8) & 0xFF); }
	if (m_channels >= 3) { *it = (byte)((int32_t)(value >> 16) & 0xFF); }
	if (m_channels >= 4) { *it = (byte)((int32_t)(value >> 24) & 0xFF); }
}

void Image::set_pixel(size_t x, size_t y, Color32 value)
{
	set_pixel(((x + y * m_width) * m_channels), value);
}