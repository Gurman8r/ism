#include <scene/resources/font.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Font, t, "font")
{
	t.tp_base = typeof<Resource>();
}

Font::~Font()
{
}

Font::Font() : Font{ get_class() }
{
}