#ifndef _ISM_EXTENSION_MANAGER_HPP_
#define _ISM_EXTENSION_MANAGER_HPP_

#include <core/extension/extension.hpp>

namespace ism
{
	class ISM_API ExtensionManager : public Object
	{
		DEFINE_CLASS(ExtensionManager, Object);

		static ExtensionManager * __singleton;

		i32 m_level{ -1 };

		HashMap<String, Ref<Extension>> m_extensions{};

	public:
		ExtensionManager() noexcept { __singleton = this; }
		virtual ~ExtensionManager() noexcept override = default;
		FORCE_INLINE static ExtensionManager * get_singleton() noexcept { return __singleton; }
#define EXTENSION_MANAGER (ism::ExtensionManager::get_singleton())

	public:
		enum LoadStatus_ {
			LoadStatus_Success,
			LoadStatus_Failure,
			LoadStatus_AlreadyLoaded,
			LoadStatus_NotLoaded,
			LoadStatus_NeedsRestart,
		};

		LoadStatus_ load_extension(String const & path);
		LoadStatus_ unload_extension(String const & path);
		bool is_extension_loaded(String const & path);
		Vector<String> get_loaded_extensions() const;
		Ref<Extension> get_extension(String const & path);

		void initialize_extensions(ExtensionInitializationLevel_ level);
		void finalize_extensions(ExtensionInitializationLevel_ level);
		void load_extensions();
	};
}

#endif // !_ISM_EXTENSION_MANAGER_HPP_
