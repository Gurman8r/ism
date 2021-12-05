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
	m_width = image->get_width();
	m_height = image->get_height();
	m_format = image->get_format();
	if (!m_texture)
	{
		m_texture = SINGLETON(RenderingServer)->texture2d_create(image);
	}
	else
	{
		RID new_texture{ SINGLETON(RenderingServer)->texture2d_create(image) };
		SINGLETON(RenderingServer)->texture_replace(m_texture, new_texture);
	}
	m_image_stored = true;
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
	if (m_image_stored)
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

EMBED_CLASS(TextureCube, t) {}

TextureCube::TextureCube() {}

TextureCube::~TextureCube() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */