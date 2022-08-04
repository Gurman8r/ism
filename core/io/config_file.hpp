#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ConfigFile : public Resource
	{
		OBJECT_CLASS(ConfigFile, Resource);

		RID m_ini;

	public:
		ConfigFile() noexcept {}

		explicit ConfigFile(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~ConfigFile();

		virtual RID get_rid() const override { return m_ini; }

		virtual Error_ reload_from_file() override;

	public:
		bool get_bool(String const & section, String const & name, bool dv = {}) const;
		
		f64 get_double(String const & section, String const & name, f64 dv = {}) const;
		
		f32 get_float(String const & section, String const & name, f32 dv = {}) const;
		
		i32 get_int(String const & section, String const & name, i32 dv = {}) const;
		
		u32 get_uint(String const & section, String const & name, u32 dv = {}) const;
		
		String get_string(String const & section, String const & name, String const & dv = {}) const;

		bool set_string(String const & section, String const & name, String const & value);
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
