#include <scene/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture, t) {}

Texture::Texture() : Resource{}
{
}

Texture::~Texture()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture2D, t) {}

Texture2D::Texture2D() : Texture{}
{
}

Texture2D::~Texture2D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture3D, t) {}

Texture3D::Texture3D() : Texture{}
{
}

Texture3D::~Texture3D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(TextureCube, t) {}

TextureCube::TextureCube() : Texture{}
{
}

TextureCube::~TextureCube()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */