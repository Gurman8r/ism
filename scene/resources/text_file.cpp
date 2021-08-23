#include <scene/resources/text_file.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(TextFile, t, "text_file")
{
	t.tp_base = typeof<Resource>();
}

TextFile::~TextFile()
{
}

TextFile::TextFile() : Resource{}
{
}