#include <scene/resources/text_file.hpp>

using namespace ism;

EMBEDED_CLASS(TextFile, t)
{
	CLASS_DEF(TextFile, t)
	{
		return t
			.def("get_text", &TextFile::get_text)
			.def("has_text", &TextFile::has_text)
			.def("load_text", &TextFile::load_text)
			.def("reload_from_file", &TextFile::reload_from_file)
			.def("set_text", &TextFile::set_text)
			.def("set_file_path", &TextFile::set_file_path)
			;
	};
}

void TextFile::reload_from_file()
{
}

Error TextFile::load_text(String const & path)
{
	return Error_None;
}