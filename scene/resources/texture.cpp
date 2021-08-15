#include <scene/resources/texture.hpp>

#ifdef ISM_RENDERER_OPENGL
#include <drivers/opengl/opengl_texture.hpp>
#endif

namespace ism
{
	Texture::~Texture()
	{
	}

	Texture::Texture()
	{
	}
}