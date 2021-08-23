#include <scene/resources/texture.hpp>

#ifdef ISM_RENDERER_OPENGL
#include <drivers/opengl/opengl_texture.hpp>
#endif

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Texture, t, "texture")
{
	t.tp_base = typeof<Resource>();
}

Texture::~Texture()
{
}

Texture::Texture() : Texture{ get_class() }
{
}