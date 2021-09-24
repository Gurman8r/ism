#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

class INIReader;

namespace ism
{
	class ISM_API ConfigFile : public Resource
	{
		OBJECT_CLASS(ConfigFile, Resource);

	public:
		ConfigFile() noexcept {}

		ConfigFile(String const & path) noexcept { open(path); }

		virtual ~ConfigFile() { close(); }

		virtual bool open(String const & path);

		virtual void close();

		NODISCARD bool good() const { return (bool)m_ini; }

		NODISCARD operator bool() const { return (bool)m_ini; }

	public:
		NODISCARD Set<String> & sections() const;
		
		NODISCARD bool get_bool(String const & section, String const & name, bool dv) const;
		
		NODISCARD float64_t get_double(String const & section, String const & name, float64_t dv) const;
		
		NODISCARD float_t get_float(String const & section, String const & name, float_t dv) const;
		
		NODISCARD int32_t get_int(String const & section, String const & name, int32_t dv) const;
		
		NODISCARD uint32_t get_uint(String const & section, String const & name, uint32_t dv) const;
		
		NODISCARD String get_string(String const & section, String const & name, String const & dv) const;

		bool set_string(String const & section, String const & name, String const & value);

	private:
		INIReader * m_ini;
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
