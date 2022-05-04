#ifndef _ISM_TEXT_SERVER_HPP_
#define _ISM_TEXT_SERVER_HPP_

#include <scene/resources/texture.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Glyph : public Texture2D
	{
		OBJECT_COMMON(Glyph, Texture2D);

		RID m_texture{};
		IntRect m_bounds{};
		uint32_t m_advance{};

	public:
		Glyph(Buffer const & buffer, IntRect const & bounds, uint32_t advance);

		virtual ~Glyph();

		Glyph(Glyph && other) noexcept { swap(std::move(other)); }

		Glyph & operator=(Glyph && other) noexcept { return swap(std::move(other)); }

		Glyph & swap(Glyph & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_texture, other.m_texture);
				m_bounds.swap(other.m_bounds);
				std::swap(m_advance, other.m_advance);
			}
			return (*this);
		}

	public:
		NODISCARD uint32_t get_advance() const noexcept { return m_advance; }

		NODISCARD virtual RID get_rid() const override { return m_texture; }
		
		NODISCARD virtual int32_t get_width() const override { return m_bounds.width(); }
		
		NODISCARD virtual int32_t get_height() const override { return m_bounds.height(); }
		
		NODISCARD virtual Ref<Image> get_image() const override;
		
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API TextServer : public Object
	{
		OBJECT_COMMON(TextServer, Object);

		static TextServer * singleton;

	public:
		explicit TextServer();

		virtual ~TextServer();

		NODISCARD static TextServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TEXT_SERVER (ism::TextServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TEXT_SERVER_HPP_
