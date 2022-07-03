#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

class INIReader;

namespace ism
{
	class ISM_API ConfigFile : public Resource
	{
		OBJECT_COMMON(ConfigFile, Resource);

		INIReader * m_ini;

	public:
		ConfigFile() noexcept {}

		explicit ConfigFile(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~ConfigFile();

		virtual RID get_rid() const override { return (RID)m_ini; }

		virtual Error_ reload_from_file() override;

	public:
		bool get_bool(String const & section, String const & name, bool dv = {}) const;
		
		double_t get_double(String const & section, String const & name, double_t dv = {}) const;
		
		float_t get_float(String const & section, String const & name, float_t dv = {}) const;
		
		int32_t get_int(String const & section, String const & name, int32_t dv = {}) const;
		
		uint32_t get_uint(String const & section, String const & name, uint32_t dv = {}) const;
		
		String get_string(String const & section, String const & name, String const & dv = {}) const;

		bool set_string(String const & section, String const & name, String const & value);
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
