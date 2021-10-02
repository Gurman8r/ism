#include <scene/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(Texture, t) {}

Texture::Texture() : Resource{}
{
}

Texture::~Texture()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(Texture2D, t) {}

Texture2D::Texture2D() : Texture{}
{
}

Texture2D::~Texture2D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(Texture3D, t) {}

Texture3D::Texture3D() : Texture{}
{
}

Texture3D::~Texture3D()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(TextureCube, t) {}

TextureCube::TextureCube() : Texture{}
{
}

TextureCube::~TextureCube()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */