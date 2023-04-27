#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>

namespace ism
{
	Error_ FontLibrary::load_font(Font & font, Path const & path)
	{
		if (path.empty()) { return Error_Unknown; }
		if (font.m_font) { TEXT_SERVER->font_destroy(font.m_font); }
		font.m_font = TEXT_SERVER->font_create(TS::FontCreateInfo{ path });
		if (!font.m_font) { return Error_Unknown; }
		font.set_path(path);
		return Error_OK;
	}

	Error_ FontLibrary::load_font(Ref<Font> font, Path const & path)
	{
		return !font ? Error_Unknown : load_font(**font, path);
	}
}