#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Font, t) {}

	Font::~Font()
	{
		if (m_font) { get_ts()->font_destroy(m_font); m_font = nullptr; }
	}

	Glyph * Font::get_glyph(u32 character, u32 character_size)
	{
		if (!m_font || !character || !character_size) { return nullptr; }

		return get_ts()->font_get_glyph(m_font, character, character_size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
