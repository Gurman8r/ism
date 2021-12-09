#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Texture, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Texture2D, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(ImageTexture, t) {}

ImageTexture::ImageTexture(Ref<Image> const & image)
{
	ASSERT(image);
	m_image = image;
	m_width = m_image->get_width();
	m_height = m_image->get_height();
	m_texture = SINGLETON(RenderingServer)->texture2d_create(m_image);
}

ImageTexture::~ImageTexture()
{
	if (m_texture)
	{
		SINGLETON(RenderingDevice)->texture_destroy(m_texture);
	}
}

Ref<Image> ImageTexture::get_data() const
{
	if (m_texture)
	{
		return SINGLETON(RenderingServer)->texture2d_get(m_texture);
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

EMBED_CLASS(Texture3D, t) {}

Texture3D::Texture3D() {}

Texture3D::~Texture3D() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Cubemap, t) {}

Cubemap::Cubemap() {}

Cubemap::~Cubemap() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */