#include <core/config/project_settings.hpp>
#include <core/os/os.hpp>
#include <core/io/file.hpp>
#include <core/io/dir.hpp>
#include <core/object/eval.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ProjectSettings, t) {}

	SINGLETON_EMBED(ProjectSettings);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ProjectSettings::setup(String const & exec_path, String const & main_pack)
	{
		if (exec_path.empty()) {
			return Error_Failed;
		}

		m_project_path = exec_path.parent_path();
		m_bin_path = m_project_path.path_join("bin");
		m_config_path = m_project_path.path_join("config");
		m_mods_path = m_project_path.path_join("mods");
		m_resource_path = m_project_path.path_join("resources");

		// engine settings
		String const engine_ini{ m_config_path.path_join("engine.ini") };
		if (!File::exists(engine_ini)) {
			PRINT_ERROR("could not locate engine settings");
		}
		else if (auto const err{ ConfigFile::parse(engine_ini, [](auto u, auto s, auto n, auto v) {
			//return (((ProjectSettings *)u)->set(String(s).path_join(n), evaluate(String(v).trim()))), true;
			return true;
		}, this) }) {
			PRINT_ERROR("could not parse engine settings");
			return Error_Failed;
		}

		// editor settings
#if TOOLS_ENABLED
		String const editor_ini{ m_config_path.path_join("editor.ini") };
		if (!File::exists(editor_ini)) {
			PRINT_ERROR("could not locate editor settings");
		}
		else if (auto const err{ ConfigFile::parse(editor_ini, [](auto u, auto s, auto n, auto v) {
			//return (((ProjectSettings *)u)->set(String(s).path_join(n), evaluate(String(v).trim()))), true;
			return true;
		}, this) }) {
			PRINT_ERROR("could not parse editor settings");
			return err;
		}
#endif

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Optional<Var> ProjectSettings::get(String const & path) const
	{
		if (auto const v{ util::getptr(m_data, path) }) {
			return *v;
		}
		else {
			return nullopt;
		}
	}

	Error_ ProjectSettings::set(String const & path, Optional<Var> const & value)
	{
		// add
		if (value) {
			return (m_data[path] = *value), Error_OK;
		}
		// remove
		else if (auto const it{ m_data.find(path) }; it != m_data.end()) {
			return m_data.erase(it), Error_OK;
		}
		// not found
		else {
			return Error_Failed;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ProjectSettings::localize_path(String const & p_path) const
	{
		if (m_resource_path.empty() || (p_path.is_absolute_path() && !p_path.begins_with(m_resource_path))) {
			return p_path.simplify_path();
		}
		
		// check if we have a special path like res:// or a protocol identifier
		size_t p{ p_path.find("://") };
		bool found{};
		if (p > 0) {
			found = true;
			for (size_t i{}; i < p; ++i) {
				if (!std::isalnum(p_path[i])) {
					found = false;
					break;
				}
			}
		}
		if (found) {
			return p_path.simplify_path();
		}
		
		Ref<Dir> dir{ Dir::create(DirAccess_Filesystem) };
		String path{ p_path.replace('\\', '/').simplify_path() };
		if (dir->change_dir(path) == Error_OK)
		{
			String cwd{ dir->get_current_dir() };
			cwd = cwd.replace('\\', '/');
			// Ensure that we end with a '/'.
			// This is important to ensure that we do not wrongly localize the resource path
			// in an absolute path that just happens to contain this string but points to a
			// different folder (e.g. "/my/project" as m_resource_path would be contained in
			// "/my/project_data", even though the latter is not part of res://.
			// `path_join("")` is an easy way to ensure we have a trailing '/'.
			String const res_path{ m_resource_path.path_join("") };
			// Dir::get_current_dir() is not guaranteed to return a path that with a trailing '/',
			// so we must make sure we have it as well in order to compare with 'res_path'.
			cwd = cwd.path_join("");
			if (!cwd.begins_with(res_path)) {
				return p_path;
			}
			return cwd.replace_first(res_path, "res://");
		}
		else
		{
			size_t sep{ path.rfind("/") };
			if (sep == String::npos) {
				return "res://" + path;
			}
			String parent{ path.substr(0, sep) };
			String plocal{ localize_path(parent) };
			if (plocal.empty()) {
				return "";
			}
			// Only strip the starting '/' from 'path' if its parent ('plocal') ends with '/'
			if (plocal[plocal.size() - 1] == '/') {
				sep += 1;
			}
			return plocal + path.substr(sep, path.size() - sep);
		}
	}

	String ProjectSettings::globalize_path(String const & path) const
	{
		if (path.begins_with("res://")) {
			if (!m_resource_path.empty()) {
				return path.replace("res:/", m_resource_path);
			}
			return path.replace("res://", "");
		}
		else if (path.begins_with("user://")) {
			if (String const data_dir{ os()->get_user_path() }; !data_dir.empty()) {
				return path.replace("user:/", data_dir);
			}
			return path.replace("user://", "");
		}
		return path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ProjectSettings::get_bin_path() const noexcept
	{
		return !m_bin_path.empty() ? m_bin_path : (m_bin_path = get_project_path().path_join("bin"));
	}

	String ProjectSettings::get_config_path() const noexcept
	{
		return !m_config_path ? m_config_path : (m_config_path = get_project_path().path_join("config"));
	}

	String ProjectSettings::get_mods_path() const noexcept
	{
		return !m_mods_path.empty() ? m_mods_path : (m_mods_path = get_project_path().path_join("mods"));
	}

	String ProjectSettings::get_project_path() const noexcept
	{
		return !m_project_path ? m_resource_path : (m_project_path = os()->get_exec_path().parent_path());
	}

	String ProjectSettings::get_resource_path() const noexcept
	{
		return !m_resource_path.empty() ? m_resource_path : (m_resource_path = get_project_path().path_join("resource"));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}