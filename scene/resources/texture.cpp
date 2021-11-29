#include <scene/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Texture, t) {}

Texture::Texture() : Resource{}
{
}

Texture::~Texture()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Texture2D, t) {}

Texture2D::Texture2D() : Texture{}
{
}

Texture2D::~Texture2D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Texture3D, t) {}

Texture3D::Texture3D() : Texture{}
{
}

Texture3D::~Texture3D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(TextureCube, t) {}

TextureCube::TextureCube() : Texture{}
{
}

TextureCube::~TextureCube()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */