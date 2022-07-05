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
		Glyph() noexcept {}

		Glyph(DynamicBuffer const & buffer, IntRect const & bounds, uint32_t advance);

		Glyph(Glyph && other) noexcept { swap(other); }

		Glyph & operator=(Glyph && other) noexcept { return swap(other); }

		virtual ~Glyph();

		Glyph & swap(Glyph & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_texture, other.m_texture);
				std::swap(m_bounds, other.m_bounds);
				std::swap(m_advance, other.m_advance);
			}
			return (*this);
		}

		uint32_t get_advance() const noexcept { return m_advance; }

		virtual RID get_rid() const override { return m_texture; }

		virtual int32_t get_width() const override { return m_bounds.width(); }

		virtual int32_t get_height() const override { return m_bounds.height(); }

		virtual Ref<Image> get_image() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API TextServer : public Object
	{
		OBJECT_COMMON(TextServer, Object);

		static TextServer * g_singleton;

	public:
		TextServer();

		virtual ~TextServer() override;

		FORCE_INLINE static TextServer * get_singleton() noexcept { return g_singleton; }

	public:
		/* FONT API */
		using FontLibrary = void *;

		using FontFace = void *;

		struct FontPage
		{
			uint32_t character_size{};

			Map<uint32_t, Glyph> glyphs{};
		};

		struct Font
		{
			Path path{};

			FontLibrary font_library{};

			FontFace font_face{};

			Map<uint32_t, FontPage> pages{};
		};

		struct FontCreateInfo
		{
			Path path{};
		};

		RID font_create(FontCreateInfo const & spec);
		void font_destroy(RID font);
		FontPage * font_get_page(RID font, uint32_t character_size);
		Glyph * font_get_glyph(RID font, uint32_t character, uint32_t character_size);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(TS) TextServer;

#define TEXT_SERVER (ism::TS::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TEXT_SERVER_HPP_
