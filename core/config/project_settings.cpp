#include <core/config/project_settings.hpp>
#include <core/object/eval.hpp>
#include <inih/INIReader.h>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ProjectSettings::setup(Path const & exepath, Path const & main_pack)
	{
		if (exepath.empty()) {
			return Error_Unknown;
		}

		// paths
		m_bin_path = "../bin/"_path;
		m_data_path = "../data/"_path;
		m_res_path = "../res/"_path;
		m_user_path = "../user/"_path;

		// engine settings
		Path const engine_ini{ get_data_path().string() + "engine.ini" };
		ini_parse(engine_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);

		// editor settings
#if TOOLS_ENABLED
		Path const editor_ini{ get_data_path().string() + "editor.ini" };
		ini_parse(editor_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);
#endif

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Path ProjectSettings::get_bin_path() const { return m_bin_path; }

	Path ProjectSettings::get_data_path() const { return m_data_path; }

	Path ProjectSettings::get_resource_path() const { return m_res_path; }

	Path ProjectSettings::get_user_path() const { return m_user_path; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJ ProjectSettings::get(String const & section, String const & name) const
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

	Error_ ProjectSettings::set(String const & section, String const & name, OBJ const & value)
	{
		// add
		if (value) {
			m_data[section][name] = value;
			return Error_OK;
		}
		// remove
		else if (auto const s{ m_data.find(section) }; s != m_data.end()) {
			if (auto const v{ s->second.find(name) }; v != s->second.end()) { s->second.erase(v); }
			else { return Error_Unknown; } // value not found
			if (s->second.empty()) { m_data.erase(s); }
			return Error_OK;
		}
		else {
			return Error_Unknown; // section not found
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}