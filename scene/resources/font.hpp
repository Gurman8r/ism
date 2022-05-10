#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <core/io/resource.hpp>
#include <servers/text_server.hpp>

namespace ism
{
	class ISM_API Font : public Resource
	{
		OBJECT_COMMON(Font, Resource);

		RID m_font;

	public:
		Font();
		
		explicit Font(Path const & path);

		virtual ~Font();

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_rid() const override { return m_font; }
	};
}

#endif // !_ISM_FONT_HPP_
