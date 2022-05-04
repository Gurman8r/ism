#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture2D, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(ImageTexture, t) {}

ImageTexture::ImageTexture(Path const & path)
{
	m_image.instance();
	if (ImageLoader::load_image(m_image, path) != Error_None) { m_image = nullptr; return; }
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RENDERING_SERVER->texture2d_create(m_image);
}

ImageTexture::ImageTexture(Ref<Image> const & image)
{
	ASSERT(image);
	m_image = image;
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RENDERING_SERVER->texture2d_create(m_image);
}

ImageTexture::~ImageTexture()
{
	if (m_texture)
	{
		RENDERING_DEVICE->texture_destroy(m_texture);
	}
}

Ref<Image> ImageTexture::get_image() const
{
	if (m_texture)
	{
		return RENDERING_SERVER->texture2d_get_image(m_texture);
	}
	else
	{
		return nullptr;
	}
}

void ImageTexture::update(Ref<Image> const & image, bool immediate)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Texture3D, t) {}

Texture3D::Texture3D() {}

Texture3D::~Texture3D() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Cubemap, t) {}

Cubemap::Cubemap() {}

Cubemap::~Cubemap() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */