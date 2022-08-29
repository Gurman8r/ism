#include <core/extension/native_extension_manager.hpp>

namespace ism
{
	EMBED_OBJECT_CLASS(NativeExtensionManager, t) {}

	NativeExtensionManager * NativeExtensionManager::__singleton{};

	NativeExtensionManager::LoadStatus_ NativeExtensionManager::load_extension(Path const & path)
	{
		LoadStatus_ status{};
		if (auto const it{ m_extensions.find(path) }; it != m_extensions.end()) {
		}
		return status;
	}

	NativeExtensionManager::LoadStatus_ NativeExtensionManager::reload_extension(Path const & path)
	{
		LoadStatus_ status{};
		return status;
	}

	NativeExtensionManager::LoadStatus_ NativeExtensionManager::unload_extension(Path const & path)
	{
		LoadStatus_ status{};
		return status;
	}

	bool NativeExtensionManager::is_extension_loaded(Path const & path)
	{
		return m_extensions.contains(path);
	}

	Vector<Path> NativeExtensionManager::get_loaded_extensions() const
	{
		Vector<Path> ext{};
		ext.reserve(m_extensions.size());
		for (auto const & [k, v] : m_extensions) {
			ext.push_back(k);
		}
		return ext;
	}

	Ref<NativeExtension> NativeExtensionManager::get_extension(Path const & path)
	{
		if (auto const it{ m_extensions.find(path) }; it != m_extensions.end()) {
			return it->second;
		}
		return nullptr;
	}

	void NativeExtensionManager::initialize_extensions(NativeExtension::InitializationLevel_ level)
	{
	}

	void NativeExtensionManager::deinitialize_extensions(NativeExtension::InitializationLevel_ level)
	{
	}

	void NativeExtensionManager::load_extensions()
	{
	}
}