#ifndef _ISM_TEXT_FILE_HPP_
#define _ISM_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API TextFile : public Resource
	{
		OBJECT_CLASS(TextFile, Resource);

		String m_text{};

	public:
		TextFile() noexcept = default;
		explicit TextFile(String const & path);
		virtual String const & get_text() const;
		virtual bool has_text(String const & compare_to = {}) const;
		virtual void set_text(String const & value);
	};
}

#endif // !_ISM_TEXT_FILE_HPP_
