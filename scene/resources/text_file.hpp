#ifndef _ISM_TEXT_FILE_HPP_
#define _ISM_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API TextFile : public Resource
	{
		OBJ_CLASS(TextFile, Resource);

		String m_text{}, m_path{};

	public:
		virtual bool has_text() const { return !m_text.empty(); }

		virtual String const & get_text() const { return m_text; }

		virtual void set_text(String const & value) { m_text = value; }

		virtual void reload_from_file() override;

		void set_file_path(String const & value) noexcept { m_path = value; }

		Error load_text(String const & path);
	};
}

#endif // !_ISM_TEXT_FILE_HPP_
