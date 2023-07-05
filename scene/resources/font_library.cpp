#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>
#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FontFormatLoader, t) {}

	Error_ FontFormatLoader::load_font(Ref<Font> font, String const & path)
	{
		if (!font) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		if (font->m_font) { get_ts()->font_destroy(font->m_font); }
		font->m_font = get_ts()->font_create(TS::FontCreateInfo{ path });
		if (!font->m_font) { return Error_Failed; }
		font->set_path(path);
		return Error_OK;
	}
	
	RES FontFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Font> temp{}; temp.instance();
		if (auto const error{ load_font(temp, get_os()->globalize_path(path)) }) { if (r_error) { *r_error = error; } temp = nullptr; }
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void FontFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".ttf");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}