#ifndef _ISM_TEXT_FILE_HPP_
#define _ISM_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API TextFile : public Resource
	{
		OBJECT_COMMON(TextFile, Resource);

		String m_text{};

	public:
		TextFile() noexcept {}

		explicit TextFile(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual Error_ reload_from_file() override;

		virtual String const & get_text() const;

		virtual bool has_text(String const & compare_to = {}) const;

		virtual void set_text(String const & value);
	};
}

#endif // !_ISM_TEXT_FILE_HPP_
