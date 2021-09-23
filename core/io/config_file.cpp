#include <core/io/config_file.hpp>

#include <inih/INIReader.h>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(ConfigFile, t, "config_file")
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool ConfigFile::open(String const & path)
{
	if (m_ini) { return false; }

	m_ini = memnew(INIReader((std::string const &)path));

	return m_ini && (m_ini->ParseError() == EXIT_SUCCESS);
}

void ConfigFile::close()
{
	if (m_ini)
	{
		memdelete(m_ini);
		
		m_ini = nullptr;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Set<String> & ConfigFile::sections() const
{
	return (Set<String> &)m_ini->Sections();
}

bool ConfigFile::get_bool(String const & section, String const & name, bool dv) const
{
	return m_ini ? m_ini->GetBoolean((std::string const &)section, (std::string const &)name, dv) : dv;
}

float64_t ConfigFile::get_double(String const & section, String const & name, float64_t dv) const
{
	return m_ini ? m_ini->GetReal((std::string const &)section, (std::string const &)name, dv) : dv;
}

float_t ConfigFile::get_float(String const & section, String const & name, float_t dv) const
{
	return (float_t)get_double(section, name, (float64_t)dv);
}

int32_t ConfigFile::get_int(String const & section, String const & name, int32_t dv) const
{
	return m_ini ? m_ini->GetInteger((std::string const &)section, (std::string const &)name, dv) : dv;
}

uint32_t ConfigFile::get_uint(String const & section, String const & name, uint32_t dv) const
{
	return (uint32_t)get_int(section, name, (int32_t)dv);
}

String ConfigFile::get_string(String const & section, String const & name, String const & dv) const
{
	return m_ini ? (String)m_ini->Get((std::string const &)section, (std::string const &)name, (std::string const &)dv) : dv;
}

bool ConfigFile::set_string(String const & section, String const & name, String const & value)
{
	return m_ini->Set((std::string const &)section, (std::string const &)name, (std::string const &)value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */