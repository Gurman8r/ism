#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API ConfigFile : public Resource
	{
		OBJECT_CLASS(ConfigFile, Resource);

	public:
		using Section = typename HashMap<String, String>;
		using Callback = typename bool(*)(void * user, cstring section, cstring name, cstring value);

	private:
		HashMap<String, Section> m_data{};

	public:
		ConfigFile() noexcept {}
		ConfigFile(ConfigFile const &) = default;
		ConfigFile(ConfigFile &&) noexcept = default;
		ConfigFile & operator=(ConfigFile const &) = default;
		ConfigFile & operator=(ConfigFile &&) noexcept = default;
		virtual ~ConfigFile() noexcept override = default;
		virtual RID get_rid() const override { return nullptr; }

	public:
		explicit ConfigFile(String const & path) { parse(path); }
		Error_ parse(String const & path);
		static Ref<ConfigFile> parse(String const & path, Error_ * r_error);
		static Error_ parse(String const & path, Callback fn, void * user);

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }

	public:
		bool set_string(String const & section, String const & name, String const & value);
		NODISCARD String get_string(String const & section, String const & name, String const & default_value = {}) const;
		NODISCARD Vector<String> get_strings(String const & section, String const & name, String const & delim, Vector<String> const & default_value = {}) const;

	public:
		NODISCARD bool get_bool(String const & section, String const & name, bool const default_value = {}) const;
		NODISCARD i32 get_i32(String const & section, String const & name, i32 const default_value = {}) const;
		NODISCARD u32 get_u32(String const & section, String const & name, u32 const default_value = {}) const;
		NODISCARD f32 get_f32(String const & section, String const & name, f32 const default_value = {}) const;
		NODISCARD f64 get_f64(String const & section, String const & name, f64 const default_value = {}) const;

	public:
		NODISCARD Vec2f get_vec2f(String const & section, String const & name, Vec2f const & default_value = {}) const;
		NODISCARD Vec3f get_vec3f(String const & section, String const & name, Vec3f const & default_value = {}) const;
		NODISCARD Vec4f get_vec4f(String const & section, String const & name, Vec4f const & default_value = {}) const;
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
