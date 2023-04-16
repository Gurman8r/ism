#include <core/extension/extension_manager.hpp>
#include <fstream>

namespace ism
{
	EMBED_CLASS(ExtensionManager, t) {}

	ExtensionManager * ExtensionManager::__singleton{};

	ExtensionManager::LoadStatus_ ExtensionManager::load_extension(Path const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it != m_extensions.end()) { return LoadStatus_AlreadyLoaded; }

		Ref<Extension> extension;
		extension.instance();
		extension->open_library(path, "initialize_mono_library");

		if (m_level >= 0) {
			i32 minimum_level{ extension->get_minimum_library_initialization_level() };
			if (minimum_level < minimum(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i = minimum_level; i <= m_level; ++i) {
				extension->initialize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions[path] = extension;
		return LoadStatus_Success;
	}

	ExtensionManager::LoadStatus_ ExtensionManager::reload_extension(Path const & path)
	{
		return LoadStatus_Success;
	}

	ExtensionManager::LoadStatus_ ExtensionManager::unload_extension(Path const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it == m_extensions.end()) { return LoadStatus_NotLoaded; }

		Ref<Extension> extension{ it->second };

		if (m_level >= 0) {
			i32 minimum_level{ extension->get_minimum_library_initialization_level() };
			if (minimum_level < minimum(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i = minimum_level; i <= m_level; ++i) {
				extension->finalize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions.erase(it);
		return LoadStatus_Success;
	}

	bool ExtensionManager::is_extension_loaded(Path const & path)
	{
		return m_extensions.contains(path);
	}

	Vector<Path> ExtensionManager::get_loaded_extensions() const
	{
		Vector<Path> ext{};
		ext.reserve(m_extensions.size());
		for (auto const & [k, v] : m_extensions) {
			ext.push_back(k);
		}
		return ext;
	}

	Ref<Extension> ExtensionManager::get_extension(Path const & path)
	{
		if (auto const it{ m_extensions.find(path) }; it != m_extensions.end()) {
			return it->second;
		}
		return nullptr;
	}

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
		std::ifstream file{ Extension::get_extension_list_config_file().c_str() };
		ON_SCOPE_EXIT(&file) { file.close(); };
		if (!file) { return; }

		String line;
		while (std::getline(file, line)) {
			load_extension(line.trim());
		}
	}
}