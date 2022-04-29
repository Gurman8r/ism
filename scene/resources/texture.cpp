#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(Texture, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(Texture2D, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(ImageTexture, t) {}

ImageTexture::ImageTexture(Path const & path)
{
	m_image.instance();
	if (ImageLoader::load_image(m_image, path) != Error_None) { m_image = nullptr; return; }
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RS->texture2d_create(m_image);
}

ImageTexture::ImageTexture(Ref<Image> const & image)
{
	ASSERT(image);
	m_image = image;
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = RS->texture2d_create(m_image);
}

ImageTexture::~ImageTexture()
{
	if (m_texture)
	{
		RD->texture_destroy(m_texture);
	}
}

Ref<Image> ImageTexture::get_data() const
{
	if (m_texture)
	{
		return RS->texture2d_get(m_texture);
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

EMBED_OBJECT_CLASS(Texture3D, t) {}

Texture3D::Texture3D() {}

Texture3D::~Texture3D() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(Cubemap, t) {}

Cubemap::Cubemap() {}

Cubemap::~Cubemap() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */