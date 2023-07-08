#ifndef _ISM_EXTENSION_MANAGER_HPP_
#define _ISM_EXTENSION_MANAGER_HPP_

#include <core/extension/extension.hpp>

namespace Ism
{
	class ISM_API ExtensionManager : public Object
	{
		DEFINE_CLASS(ExtensionManager, Object);

		static ExtensionManager * __singleton;

		String m_config_file_path{};

		i32 m_level{ -1 };

		HashMap<String, Ref<Extension>> m_extensions{};

	public:
		ExtensionManager();
		virtual ~ExtensionManager() override;
		SINGLETON_GETTER(ExtensionManager, __singleton);

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

		NODISCARD bool is_extension_loaded(String const & path);
		NODISCARD Vector<String> get_loaded_extensions() const;
		NODISCARD Ref<Extension> get_extension(String const & path);

		void load_extensions();
		void initialize_extensions(ExtensionInitializationLevel_ level);
		void finalize_extensions(ExtensionInitializationLevel_ level);
	};

	SINGLETON_WRAPPER(ExtensionManager, get_ext);
}

#endif // !_ISM_EXTENSION_MANAGER_HPP_
