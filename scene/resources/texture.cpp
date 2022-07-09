#include <scene/resources/texture.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Texture, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Texture2D, t) {}

	Vec2i Texture2D::get_size() const
	{
		return { get_width(), get_height() };
	}

	bool Texture2D::is_pixel_opaque(int32_t x, int32_t y) const
	{
		return true;
	}

	Ref<Image> Texture2D::get_data() const
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ImageTexture, t) {}

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

	int32_t ImageTexture::get_width() const
	{
		return m_width;
	}

	int32_t ImageTexture::get_height() const
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

	OBJECT_EMBED(Texture3D, t) {}

	Texture3D::Texture3D() {}

	Texture3D::~Texture3D() {}

	RID Texture3D::get_rid() const
	{
		return RID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(TextureCube, t) {}

	TextureCube::TextureCube() {}

	TextureCube::~TextureCube() {}

	RID TextureCube::get_rid() const
	{
		return RID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}