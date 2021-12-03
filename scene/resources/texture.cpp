#include <scene/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Texture, t) {}

Texture::Texture() : Resource{}
{
}

Texture::~Texture()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Texture2D, t) {}

Texture2D::Texture2D() : Texture{}
{
}

Texture2D::~Texture2D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Texture3D, t) {}

Texture3D::Texture3D() : Texture{}
{
}

Texture3D::~Texture3D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(TextureCube, t) {}

TextureCube::TextureCube() : Texture{}
{
}

TextureCube::~TextureCube()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */