#include <core/io/config_file.hpp>
#include <inih/INIReader.h>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ConfigFile, t) {}

	ConfigFile::~ConfigFile() {}

	Error_ ConfigFile::reload_from_file()
	{
		if (get_path().empty()) { return Error_Unknown; }
		if (!ini_parse(get_path().c_str(), [
		](void * user, cstring section, cstring name, cstring value) -> i32 {
			auto const self{ VALIDATE((ConfigFile *)user) };
			self->m_sections.insert(section);
			String const key{ String{ section } + '=' + String{ name } };
			String temp{ value };
			if (temp.size() >= 2 && temp.front() == '\"' && temp.back() == '\"') {
				temp.pop_back();
				temp.erase(temp.begin());
			}
			self->m_values.insert({ key, std::move(temp) });
			return 1;
		}, this)) { return Error_Unknown; }
		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::get_bool(String const & section, String const & name, bool default_value) const
	{
		if (auto const v{ util::to_bool(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	i32 ConfigFile::get_i32(String const & section, String const & name, i32 default_value) const
	{
		if (auto const v{ util::to_i32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	u32 ConfigFile::get_u32(String const & section, String const & name, u32 default_value) const
	{
		if (auto const v{ util::to_u32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f32 ConfigFile::get_f32(String const & section, String const & name, f32 default_value) const
	{
		if (auto const v{ util::to_f32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f64 ConfigFile::get_f64(String const & section, String const & name, f64 default_value) const
	{
		if (auto const v{ util::to_f64(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	String ConfigFile::get_string(String const & section, String const & name, String const & default_value) const
	{
		String const key{ String{ section } + '=' + String{ name } };
		if (auto const v{ util::getptr(m_values, key) }) { return *v; }
		else { return default_value; }
	}

	bool ConfigFile::set_string(String const & section, String const & name, String const & value)
	{
		String const key{ String{ section } + '=' + String{ name } };
		m_sections.insert(section);
		return m_values.insert({ key, value }).second;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}