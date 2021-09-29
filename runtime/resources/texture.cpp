#include <runtime/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(Texture, t) {}

Texture::Texture() : Resource{}
{
}

Texture::~Texture()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(Texture2D, t) {}

Texture2D::Texture2D() : Texture{}
{
}

Texture2D::~Texture2D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(Texture3D, t) {}

Texture3D::Texture3D() : Texture{}
{
}

Texture3D::~Texture3D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(TextureCube, t) {}

TextureCube::TextureCube() : Texture{}
{
}

TextureCube::~TextureCube()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */