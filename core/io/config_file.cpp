#include <core/io/config_file.hpp>
#include <inih/INIReader.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ConfigFile, t) {}

	ConfigFile::~ConfigFile() {}

	ConfigFile::ConfigFile(String const & path)
	{
		if (path.empty()) { return; }
		if (ini_parse(path.c_str(), [](auto user, auto section, auto name, auto value)
		{
			auto v{ String(value).trim([](char c) { return c == ' ' || c == '\'' || c == '\"'; }) };
			((ConfigFile *)user)->set_string(section, name, v);
			return 1;
		}
		, this)) { set_path(path); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ConfigFile::get_string(String const & section, String const & name, String const & default_value) const
	{
		Section const * s;
		String const * v;
		if ((s = util::getptr(m_data, section)) && (v = util::getptr(*s, name))) { return *v; }
		else { return default_value; }
	}

	Vector<String> ConfigFile::get_strings(String const & section, String const & name, String const & delim, Vector<String> const & default_value) const
	{
		String s{ get_string(section, name) };
		if (s.empty()) { return default_value; }
		Vector<String> v{ s.split(delim) };
		if (v.empty()) { return default_value; }
		return v;
	}

	bool ConfigFile::set_string(String const & section, String const & name, String const & value)
	{
		return m_data[section].insert({ name, value }).second;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::get_bool(String const & section, String const & name, bool default_value) const
	{
		if (auto const v{ util::to_bool(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	i32 ConfigFile::get_i32(String const & section, String const & name, i32 default_value) const
	{
		if (auto const v{ util::to_i32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	u32 ConfigFile::get_u32(String const & section, String const & name, u32 default_value) const
	{
		if (auto const v{ util::to_u32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f32 ConfigFile::get_f32(String const & section, String const & name, f32 default_value) const
	{
		if (auto const v{ util::to_f32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f64 ConfigFile::get_f64(String const & section, String const & name, f64 default_value) const
	{
		if (auto const v{ util::to_f64(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vec2f ConfigFile::get_vec2f(String const & section, String const & name, Vec2f const & default_value) const
	{
		Vec2f temp;
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: {
			temp = default_value;
		} break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			temp[1] = default_value[1];
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
		Vec3f temp;
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: {
			temp = default_value;
		} break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			temp[1] = default_value[1];
			temp[2] = default_value[2];
		} break;
		case 2: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			temp[2] = default_value[2];
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
		Vec4f temp;
		switch (Vector<String> v{ get_strings(section, name, ",") }; v.size())
		{
		case 0: {
			temp = default_value;
		} break;
		case 1: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			temp[1] = default_value[1];
			temp[2] = default_value[2];
			temp[3] = default_value[3];
		} break;
		case 2: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			temp[2] = default_value[2];
			temp[3] = default_value[3];
		} break;
		case 3: {
			if (auto const x{ util::to_f32(v[0]) }) { temp[0] = *x; } else { temp[0] = default_value[0]; }
			if (auto const y{ util::to_f32(v[1]) }) { temp[1] = *y; } else { temp[1] = default_value[1]; }
			if (auto const z{ util::to_f32(v[2]) }) { temp[2] = *z; } else { temp[2] = default_value[2]; }
			temp[3] = default_value[3];
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