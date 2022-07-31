#include <scene/resources/text_file.hpp>
#include <servers/text_server.hpp>
#include <fstream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(TextFile, t)
	{
		t.tp_bind = CLASS_INSTALLER(TextFile, t)
		{
			return t
				.def("reload_from_file", &TextFile::reload_from_file)
				.def("get_text", &TextFile::get_text)
				.def("has_text", &TextFile::has_text)
				.def("set_text", &TextFile::set_text)
				;
		};
	}

	Error_ TextFile::reload_from_file()
	{
		if (!get_path()) { return Error_Unknown; }

		// open file
		std::ifstream file{ get_path().c_str(), std::ios_base::binary };
		ON_SCOPE_EXIT(&) { file.close(); };
		if (!file) { return Error_Unknown; }

		// load contents
		file.seekg(0, std::ios_base::end);
		if (std::streampos size{ file.tellg() }; 0 < size) {
			file.seekg(0, std::ios_base::beg);
			m_text.resize((size_t)size);
			file.read((char *)m_text.data(), size);
		}

		return Error_None;
	}

	String const & TextFile::get_text() const
	{
		return m_text;
	}

	bool TextFile::has_text(String const & compare_to) const
	{
		return compare_to.empty() ? m_text.empty() : (m_text == compare_to);
	}

	void TextFile::set_text(String const & value)
	{
		m_text = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}