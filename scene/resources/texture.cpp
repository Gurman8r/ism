#include <scene/resources/texture.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture2D, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(ImageTexture, t) {}

ImageTexture::ImageTexture(Path const & path)
{
	m_image.instance(path);
	if (!m_image->get_capacity()) { m_image = nullptr; return; }
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RENDERING_SERVER->texture2d_create(m_image);
}

ImageTexture::ImageTexture(Ref<Image> const & image)
{
	m_image = VALIDATE(image);
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RENDERING_SERVER->texture2d_create(m_image);
}

ImageTexture::~ImageTexture()
{
	if (m_texture) { RENDERING_DEVICE->texture_destroy(m_texture); m_texture = nullptr; }
}

Ref<Image> ImageTexture::get_image() const
{
	return m_texture ? RENDERING_SERVER->texture2d_get_image(m_texture) : nullptr;
}

void ImageTexture::update(Ref<Image> const & image, bool immediate)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture3D, t) {}

Texture3D::Texture3D() {}

Texture3D::~Texture3D() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(TextureCube, t) {}

TextureCube::TextureCube() {}

TextureCube::~TextureCube() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */