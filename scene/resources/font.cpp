#include <scene/resources/font.hpp>
#include <servers/text_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Font, t) {}

	Font::~Font()
	{
		if (m_font) { TS::get_singleton()->font_destroy(m_font); m_font = nullptr; }
	}

	Error_ Font::reload_from_file()
	{
		if (!get_path()) { return Error_Unknown; }

		if (m_font) { TS::get_singleton()->font_destroy(m_font); }

		m_font = TS::get_singleton()->font_create(TS::FontCreateInfo{ get_path() });

		if (!m_font) { return Error_Unknown; }

		return Error_None;
	}

	Glyph * Font::get_glyph(uint32_t character, uint32_t character_size)
	{
		if (!m_font || !character || !character_size) { return nullptr; }

		return TS::get_singleton()->font_get_glyph(m_font, character, character_size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
