#include <core/io/config_file.hpp>

#include <inih/INIReader.h>

#define INI(ini) ((INIReader *)(ini))

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ConfigFile, t) {}

	ConfigFile::~ConfigFile()
	{
		if (m_ini) { memdelete(INI(m_ini)); m_ini = nullptr; }
	}

	Error_ ConfigFile::reload_from_file()
	{
		if (m_ini) { memdelete(INI(m_ini)); m_ini = nullptr; }

		if (!get_path()) { return Error_Unknown; }

		m_ini = (RID)memnew(INIReader(get_path().c_str()));

		if (INI(m_ini)->ParseError() != EXIT_SUCCESS) {
			memdelete(INI(m_ini));
			m_ini = nullptr;
			return Error_Unknown;
		}

		return Error_None;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::get_bool(String const & section, String const & name, bool dv) const
	{
		return INI(m_ini)->GetBoolean((std::string const &)section, (std::string const &)name, dv);
	}

	f64 ConfigFile::get_double(String const & section, String const & name, f64 dv) const
	{
		return INI(m_ini)->GetReal((std::string const &)section, (std::string const &)name, dv);
	}

	f32 ConfigFile::get_float(String const & section, String const & name, f32 dv) const
	{
		return (f32)get_double(section, name, (f64)dv);
	}

	i32 ConfigFile::get_int(String const & section, String const & name, i32 dv) const
	{
		return INI(m_ini)->GetInteger((std::string const &)section, (std::string const &)name, dv);
	}

	u32 ConfigFile::get_uint(String const & section, String const & name, u32 dv) const
	{
		return (u32)get_int(section, name, (i32)dv);
	}

	String ConfigFile::get_string(String const & section, String const & name, String const & dv) const
	{
		return (String)INI(m_ini)->Get((std::string const &)section, (std::string const &)name, (std::string const &)dv);
	}

	bool ConfigFile::set_string(String const & section, String const & name, String const & value)
	{
		return INI(m_ini)->Set((std::string const &)section, (std::string const &)name, (std::string const &)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}