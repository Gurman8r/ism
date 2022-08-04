#include <scene/resources/texture.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Texture, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Texture2D, t) {}

	Vec2i Texture2D::get_size() const
	{
		return { get_width(), get_height() };
	}

	bool Texture2D::is_pixel_opaque(i32 x, i32 y) const
	{
		return true;
	}

	Ref<Image> Texture2D::get_data() const
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(ImageTexture, t) {}

	ImageTexture::ImageTexture(Path const & path)
	{
		m_image_cache.instance(path);
		if (!m_image_cache->get_capacity()) { m_image_cache = nullptr; return; }
		m_width = m_image_cache->get_width();
		m_height = m_image_cache->get_height();
		m_texture = RS::get_singleton()->texture2d_create(m_image_cache);
	}

	ImageTexture::ImageTexture(Ref<Image> const & image)
	{
		m_image_cache = VALIDATE(image);
		m_width = m_image_cache->get_width();
		m_height = m_image_cache->get_height();
		m_texture = RS::get_singleton()->texture2d_create(m_image_cache);
	}

	ImageTexture::~ImageTexture()
	{
		if (m_texture) { RD::get_singleton()->texture_destroy(m_texture); m_texture = nullptr; }
	}

	RID ImageTexture::get_rid() const
	{
		return m_texture;
	}

	bool ImageTexture::has_alpha() const
	{
		return m_image_cache && (3 < m_image_cache->get_depth());
	}

	i32 ImageTexture::get_width() const
	{
		return m_width;
	}

	i32 ImageTexture::get_height() const
	{
		return m_height;
	}

	Ref<Image> ImageTexture::get_data() const
	{
		return m_texture ? RS::get_singleton()->texture2d_get_data(m_texture) : nullptr;
	}

	void ImageTexture::update(Ref<Image> const & image, bool immediate)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Texture3D, t) {}

	Texture3D::Texture3D() {}

	Texture3D::~Texture3D() {}

	RID Texture3D::get_rid() const
	{
		return RID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(TextureCube, t) {}

	TextureCube::TextureCube() {}

	TextureCube::~TextureCube() {}

	RID TextureCube::get_rid() const
	{
		return RID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}