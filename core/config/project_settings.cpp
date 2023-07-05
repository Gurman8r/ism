#include <core/config/project_settings.hpp>
#include <core/os/os.hpp>
#include <core/object/eval.hpp>
#include <inih/INIReader.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ProjectSettings::setup(String const & exepath, String const & main_pack)
	{
		if (exepath.empty()) {
			return Error_Failed;
		}

		// engine settings
		String const engine_ini{ get_config_path() + "engine.ini" };
		ini_parse(engine_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);

#if TOOLS_ENABLED
		// editor settings
		String const editor_ini{ get_config_path() + "editor.ini" };
		ini_parse(editor_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);
#endif

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ProjectSettings::globalize_path(String const & path) const
	{
		if (path.has_prefix("res://")) {
			if (!m_resources_path.empty()) {
				return path.replace("res:/", m_resources_path);
			}
			return path.replace("res://", "");
		}
		else if (path.has_prefix("user://")) {
			if (String data_dir{ get_os()->get_user_path() }; !data_dir.empty()) {
				return path.replace("user:/", data_dir);
			}
			return path.replace("user://", "");
		}
		return path;
	}

	String ProjectSettings::get_bin_path() const { return m_bin_path; }

	String ProjectSettings::get_config_path() const { return m_config_path; }

	String ProjectSettings::get_cache_path() const { return m_cache_path; }

	String ProjectSettings::get_data_path() const { return m_data_path; }

	String ProjectSettings::get_downloads_path() const { return m_downloads_path; }

	String ProjectSettings::get_mods_path() const { return m_mods_path; }

	String ProjectSettings::get_profiles_path() const { return m_profiles_path; }

	String ProjectSettings::get_resources_path() const { return m_resources_path; }

	String ProjectSettings::get_saves_path() const { return m_saves_path; }

	String ProjectSettings::get_user_path() const { return m_user_path; }

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