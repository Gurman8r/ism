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

		explicit ConfigFile(Path const & path);

		virtual ~ConfigFile();

		NODISCARD virtual RID get_rid() const override { return (RID)m_ini; }

		virtual Error_ reload_from_file() override;

	public:
		NODISCARD bool get_bool(String const & section, String const & name, bool dv) const;
		
		NODISCARD double_t get_double(String const & section, String const & name, double_t dv) const;
		
		NODISCARD float_t get_float(String const & section, String const & name, float_t dv) const;
		
		NODISCARD int32_t get_int(String const & section, String const & name, int32_t dv) const;
		
		NODISCARD uint32_t get_uint(String const & section, String const & name, uint32_t dv) const;
		
		NODISCARD String get_string(String const & section, String const & name, String const & dv) const;

		bool set_string(String const & section, String const & name, String const & value);
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
