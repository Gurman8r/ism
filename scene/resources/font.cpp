#include <scene/resources/font.hpp>
#include <servers/text_server.hpp>

using namespace ism;

OBJECT_EMBED(Font, t) {}

Font::Font()
{
}

Font::Font(Path const & path)
{
	set_path(path);

	reload_from_file();
}

Font::~Font()
{
	if (m_font) { TEXT_SERVER->font_destroy(m_font); m_font = nullptr; }
}

Error_ Font::reload_from_file()
{
	if (!get_path()) { return Error_Unknown; }

	if (m_font) { TEXT_SERVER->font_destroy(m_font); }

	m_font = TEXT_SERVER->font_create(TS::FontCreateInfo{ get_path() });

	if (!m_font) { return Error_Unknown; }

	return Error_None;
}