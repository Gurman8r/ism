#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>
#include <core/config/project_settings.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(FontFormatLoader, t) {}

	Error_ FontFormatLoader::load_font(Ref<Font> font, String const & path)
	{
		if (!font) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		if (font->m_font) { text_server()->font_destroy(font->m_font); }
		font->m_font = text_server()->font_create(TS::FontCreateInfo{ path });
		if (!font->m_font) { return Error_Failed; }
		font->set_path(path);
		return Error_OK;
	}
	
	RES FontFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Font> temp{}; temp.instance();
		if (Error_ const err{ load_font(temp, globals()->globalize_path(path)) }) {
			if (r_error) { *r_error = err; }
			temp = nullptr;
		}
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void FontFormatLoader::get_recognized_extensions(Vector<String> * r_out) const
	{
		if (!r_out) { return; }
		r_out->push_back(".ttf");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}