#include <scene/resources/text_file.hpp>

using namespace ism;

OBJ_IMPL(TextFile, t) {}

void TextFile::reload_from_file()
{
}

Error TextFile::load_text(String const & path)
{
	return Error_None;
}