#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <servers/text_server.hpp>

namespace ism
{
	class ISM_API Font : public Resource
	{
		DEFINE_CLASS(Font, Resource);

		RID m_font;

	public:
		Font() noexcept {}
		
		explicit Font(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Font();

		virtual Error_ reload_from_file() override;

		virtual RID get_rid() const override { return m_font; }

		virtual Glyph * get_glyph(u32 character, u32 character_size);
	};
}

#endif // !_ISM_FONT_HPP_
