#ifndef _ISM_TEXT_FILE_HPP_
#define _ISM_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API TextFile : public Resource
	{
		OBJECT_COMMON(TextFile, Resource);

		String m_text{}, m_path{};

	public:
		virtual String const & get_text() const { return m_text; }

		virtual bool has_text() const { return !m_text.empty(); }

		Error load_text(String const & path);

		virtual void reload_from_file() override;

		virtual void set_text(String const & value) { m_text = value; }

		void set_file_path(String const & value) { m_path = value; }
	};
}

#endif // !_ISM_TEXT_FILE_HPP_
