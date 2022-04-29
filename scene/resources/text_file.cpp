#include <scene/resources/text_file.hpp>

using namespace ism;

EMBED_OBJECT_CLASS(TextFile, t)
{
	t.tp_bind = CLASS_BINDER(TextFile, t)
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

Error_ TextFile::load_text(Path const & path)
{
	return Error_None;
}