#include <core/io/config_file.hpp>

using namespace ism;

#include <inih/INIReader.h>

#define INI(ini) ((INIReader *)(ini))

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

double_t ConfigFile::get_double(String const & section, String const & name, double_t dv) const
{
	return INI(m_ini)->GetReal((std::string const &)section, (std::string const &)name, dv);
}

float_t ConfigFile::get_float(String const & section, String const & name, float_t dv) const
{
	return (float_t)get_double(section, name, (float64_t)dv);
}

int32_t ConfigFile::get_int(String const & section, String const & name, int32_t dv) const
{
	return INI(m_ini)->GetInteger((std::string const &)section, (std::string const &)name, dv);
}

uint32_t ConfigFile::get_uint(String const & section, String const & name, uint32_t dv) const
{
	return (uint32_t)get_int(section, name, (int32_t)dv);
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