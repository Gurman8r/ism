#include <core/config/project_settings.hpp>
#include <core/io/file.hpp>
#include <core/os/os.hpp>
#include <core/object/eval.hpp>
#include <inih/INIReader.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ProjectSettings::setup(String const & exe_path, String const & main_pack)
	{
		if (exe_path.empty()) {
			return Error_Failed;
		}

		String const engine_ini{ get_os()->get_config_dir().path_join("engine.ini") };
		if (File::exists(engine_ini) && !ini_parse(engine_ini.c_str(), [](auto u, auto s, auto n, auto v) { return (((ProjectSettings *)u)->set(s, n, evaluate(String(v).trim([](i32 c) { return c == ' ' || c == '\'' || c == '\"'; })))), 1; }, this)) {
			return Error_Failed;
		}

		String const editor_ini{ get_os()->get_config_dir().path_join("editor.ini") };
		if (File::exists(editor_ini) && !ini_parse(editor_ini.c_str(), [](auto u, auto s, auto n, auto v) { return (((ProjectSettings *)u)->set(s, n, evaluate(String(v).trim([](i32 c) { return c == ' ' || c == '\'' || c == '\"'; })))), 1; }, this)) {
			return Error_Failed;
		}

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ObjectRef ProjectSettings::get(String const & section, String const & name) const
	{
		if (auto const s{ util::getptr(m_data, section) })
		{
			if (auto const v{ util::getptr(*s, name) })
			{
				return *v;
			}
		}
		return nullptr;
	}

	Error_ ProjectSettings::set(String const & section, String const & name, ObjectRef const & value)
	{
		// add
		if (value) {
			m_data[section][name] = value;
			return Error_OK;
		}
		// remove
		else if (auto const s{ m_data.find(section) }; s != m_data.end()) {
			if (auto const v{ s->second.find(name) }; v != s->second.end()) { s->second.erase(v); }
			else { return Error_Failed; } // value not found
			if (s->second.empty()) { m_data.erase(s); }
			return Error_OK;
		}
		else {
			return Error_Failed; // section not found
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}