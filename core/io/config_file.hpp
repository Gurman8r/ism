#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API ConfigFile : public Resource
	{
		DEFINE_CLASS(ConfigFile, Resource);

		using Section = HashMap<String, String>;
		HashMap<String, Section> m_data{};

	public:
		ConfigFile() noexcept {}
		explicit ConfigFile(String const & path);
		virtual ~ConfigFile();
		virtual RID get_rid() const override { return 0; }

	public:
		NODISCARD String get_string(String const & section, String const & name, String const & default_value = {}) const;
		bool set_string(String const & section, String const & name, String const & value);

	public:
		NODISCARD bool get_bool(String const & section, String const & name, bool default_value = {}) const;
		NODISCARD i32 get_i32(String const & section, String const & name, i32 default_value = {}) const;
		NODISCARD u32 get_u32(String const & section, String const & name, u32 default_value = {}) const;
		NODISCARD f32 get_f32(String const & section, String const & name, f32 default_value = {}) const;
		NODISCARD f64 get_f64(String const & section, String const & name, f64 default_value = {}) const;

	public:
		NODISCARD Vec2f get_vec2f(String const & section, String const & name, Vec2f const & default_value = {}) const;
		NODISCARD Vec3f get_vec3f(String const & section, String const & name, Vec3f const & default_value = {}) const;
		NODISCARD Vec4f get_vec4f(String const & section, String const & name, Vec4f const & default_value = {}) const;
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
