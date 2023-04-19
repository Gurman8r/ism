#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ConfigFile : public Resource
	{
		DEFINE_CLASS(ConfigFile, Resource);

		Set<String> m_sections{};
		Map<String, String> m_values{};

	public:
		ConfigFile() noexcept {}
		explicit ConfigFile(Path const & path) noexcept { set_path(path); reload_from_file(); }
		virtual ~ConfigFile();
		virtual Error_ reload_from_file() override;
		virtual RID get_rid() const override { return 0; }

	public:
		NODISCARD auto get_sections() const noexcept -> Set<String> const & { return m_sections; }
		NODISCARD auto get_values() const noexcept -> Map<String, String> const & { return m_values; }
		NODISCARD bool get_bool(String const & section, String const & name, bool default_value = {}) const;
		NODISCARD i32 get_i32(String const & section, String const & name, i32 default_value = {}) const;
		NODISCARD u32 get_u32(String const & section, String const & name, u32 default_value = {}) const;
		NODISCARD f32 get_f32(String const & section, String const & name, f32 default_value = {}) const;
		NODISCARD f64 get_f64(String const & section, String const & name, f64 default_value = {}) const;
		NODISCARD String get_string(String const & section, String const & name, String const & default_value = {}) const;
		bool set_string(String const & section, String const & name, String const & value);
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
