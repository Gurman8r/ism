#include <core/io/config_file.hpp>
#include <inih/INIReader.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ConfigFile, t) {}

	Error_ ConfigFile::parse(String const & path)
	{
		if (path.empty()) {
			return Error_Failed;
		}
		else if (parse(path, [](auto u, auto s, auto n, auto v) {
			return (((ConfigFile *)u)->set_string(s, n, String(v).trim([](i32 c) { return c == ' ' || c == '\'' || c == '\"'; }))), true;
		}, this)) {
			return Error_Failed;
		}
		else {
			return set_path(path), Error_OK;
		}
	}

	Ref<ConfigFile> ConfigFile::parse(String const & path, Error_ * r_error)
	{
		Ref<ConfigFile> cfg; cfg.instance();
		if (Error_ const err{ cfg->parse(path) }) {
			if (r_error) { *r_error = Error_Failed; }
			cfg = nullptr;
		}
		else if (r_error) { *r_error = Error_OK; }
		return cfg;
	}

	Error_ ConfigFile::parse(String const & path, Callback fn, void * user)
	{
		return ini_parse(path.c_str(), (ini_handler)fn, user) ? Error_Failed : Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::set_string(String const & section, String const & name, String const & value)
	{
		return m_data[section].insert({ name, value }).second;
	}

	String ConfigFile::get_string(String const & section, String const & name, String const & default_value) const
	{
		Section const * s;
		String const * v;
		if ((s = util::getptr(m_data, section)) && (v = util::getptr(*s, name))) {
			return *v;
		}
		else {
			return default_value;
		}
	}

	Vector<String> ConfigFile::get_strings(String const & section, String const & name, String const & delim, Vector<String> const & default_value) const
	{
		if (String const s{ get_string(section, name) }; s.empty()) {
			return default_value;
		}
		else if (Vector<String> const v{ s.split(delim) }; v.empty()) {
			return default_value;
		}
		else {
			return v;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::get_bool(String const & section, String const & name, bool const default_value) const
	{
		if (auto const v{ util::to_bool(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	i32 ConfigFile::get_i32(String const & section, String const & name, i32 const default_value) const
	{
		if (auto const v{ util::to_i32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	u32 ConfigFile::get_u32(String const & section, String const & name, u32 const default_value) const
	{
		if (auto const v{ util::to_u32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f32 ConfigFile::get_f32(String const & section, String const & name, f32 const default_value) const
	{
		if (auto const v{ util::to_f32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f64 ConfigFile::get_f64(String const & section, String const & name, f64 const default_value) const
	{
		if (auto const v{ util::to_f64(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vec2f ConfigFile::get_vec2f(String const & section, String const & name, Vec2f const & default_value) const
	{
		Vec2f temp{ default_value };
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
		} break;
		default: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
		} break;
		}
		return temp;
	}

	Vec3f ConfigFile::get_vec3f(String const & section, String const & name, Vec3f const & default_value) const
	{
		Vec3f temp{ default_value };
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
		} break;
		case 2: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
		} break;
		default: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			if (auto const z{ util::to_f32(v[2]) }) { temp[2] = *z; } else { temp[2] = default_value[2]; }
		} break;
		}
		return temp;
	}

	Vec4f ConfigFile::get_vec4f(String const & section, String const & name, Vec4f const & default_value) const
	{
		Vec4f temp{ default_value };
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
		} break;
		case 2: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
		} break;
		case 3: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			if (auto const z{ util::to_f32(v[2]) }) { temp[2] = *z; } else { temp[2] = default_value[2]; }
		} break;
		default: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			if (auto const z{ util::to_f32(v[2]) }) { temp[2] = *z; } else { temp[2] = default_value[2]; }
			if (auto const w{ util::to_f32(v[3]) }) { temp[3] = *w; } else { temp[3] = default_value[3]; }
		} break;
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}