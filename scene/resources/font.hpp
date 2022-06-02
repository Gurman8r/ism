#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <servers/text_server.hpp>

namespace ism
{
	class ISM_API Font : public Resource
	{
		OBJECT_COMMON(Font, Resource);

		RID m_font;

	public:
		Font() noexcept {}
		
		explicit Font(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Font();

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_rid() const override { return m_font; }

		NODISCARD virtual Glyph * get_glyph(uint32_t character, uint32_t character_size);
	};
}

#endif // !_ISM_FONT_HPP_
