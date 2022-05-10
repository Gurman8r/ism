#include <scene/resources/text_file.hpp>
#include <servers/text_server.hpp>

using namespace ism;

OBJECT_EMBED(TextFile, t)
{
	t.tp_bind = CLASS_BINDER(TextFile, t)
	{
		return t
			.def("reload_from_file", &TextFile::reload_from_file)
			.def("get_text", &TextFile::get_text)
			.def("has_text", &TextFile::has_text)
			.def("set_text", &TextFile::set_text)
			;
	};
}

TextFile::TextFile(Path const & path)
{
	set_path(path);

	reload_from_file();
}

Error_ TextFile::reload_from_file()
{
	if (!get_path()) { return Error_Unknown; }

	return Error_None;
}

String const & TextFile::get_text() const
{
	return m_text;
}

bool TextFile::has_text(String const & compare_to) const
{
	return compare_to.empty() ? m_text.empty() : (m_text == compare_to);
}

void TextFile::set_text(String const & value)
{
	m_text = value;
}