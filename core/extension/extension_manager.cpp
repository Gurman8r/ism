#include <core/extension/extension_manager.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file_access.hpp>
#include <core/io/config_file.hpp>
#include <core/io/resource_loader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ExtensionManager, t) {}

	ExtensionManager * ExtensionManager::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ExtensionManager::LoadStatus_ ExtensionManager::load_extension(String const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it != m_extensions.end()) { return LoadStatus_AlreadyLoaded; }

		String const stem{ path.stem() };
		String const ini_path{ String::format("%s%s.ini", PROJECT_SETTINGS->get_config_path().c_str(), stem.c_str()) };
		ConfigFile const ini{ ini_path };
		String const library_name{ ini.get_string("configuration", "library_name", stem) };
		String const entry_symbol{ ini.get_string("configuration", "entry_symbol", String::format("open_%s_library", library_name.c_str())) };
		String const dll_path{ String::format("%s%s", PROJECT_SETTINGS->get_binary_path().c_str(), library_name.c_str()) };
		Ref<Extension> extension{ Extension::open(dll_path, entry_symbol) };
		if (!extension) { return LoadStatus_Failure; }

		if (m_level >= 0) {
			i32 const minimum_level{ extension->get_minimum_level() };
			if (minimum_level < MIN(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i{ minimum_level }; i <= m_level; ++i) {
				extension->initialize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions[path] = extension;
		return LoadStatus_Success;
	}

	ExtensionManager::LoadStatus_ ExtensionManager::unload_extension(String const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it == m_extensions.end()) { return LoadStatus_NotLoaded; }

		Ref<Extension> extension{ it->second };

		if (m_level >= 0) {
			i32 const minimum_level{ extension->get_minimum_level() };
			if (minimum_level < MIN(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i{ minimum_level }; i <= m_level; ++i) {
				extension->finalize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions.erase(it);
		return LoadStatus_Success;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ExtensionManager::is_extension_loaded(String const & path)
	{
		return m_extensions.contains(path);
	}

	Vector<String> ExtensionManager::get_loaded_extensions() const
	{
		Vector<String> ext{};
		ext.reserve(m_extensions.size());
		for (auto const & [k, v] : m_extensions) {
			ext.push_back(k);
		}
		return ext;
	}

	Ref<Extension> ExtensionManager::get_extension(String const & path)
	{
		if (auto const it{ m_extensions.find(path) }; it != m_extensions.end()) {
			return it->second;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ExtensionManager::initialize_extensions(ExtensionInitializationLevel_ level)
	{
		for (auto & [k, v] : m_extensions) {
			v->initialize_library(level);
		}
		m_level = level;
	}

	void ExtensionManager::finalize_extensions(ExtensionInitializationLevel_ level)
	{
		for (auto & [k, v] : m_extensions) {
			v->finalize_library(level);
		}
		m_level = (i32)level - 1;
	}

	void ExtensionManager::load_extensions()
	{
		auto f{ FileAccess::open(Extension::get_extension_list_config_file(), FileMode_Read) };
		if (!f) { return; }
		while (String line{ f->read_line().trim() }) {
			load_extension(line);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}