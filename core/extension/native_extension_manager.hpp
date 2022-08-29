#ifndef _ISM_NATIVE_EXTENSION_MANAGER_HPP_
#define _ISM_NATIVE_EXTENSION_MANAGER_HPP_

// PLACEHOLDER

#include <core/extension/native_extension.hpp>

namespace ism
{
	class ISM_API NativeExtensionManager : public Object
	{
		OBJECT_CLASS(NativeExtensionManager, Object);

		static NativeExtensionManager * __singleton;

		i32 m_level{ -1 };

		HashMap<String, Ref<NativeExtension>> m_extensions{};

	public:
		NativeExtensionManager() { __singleton = this; }

		enum LoadStatus_
		{
			LoadStatus_Success,
			LoadStatus_Failure,
			LoadStatus_AlreadyLoaded,
			LoadStatus_NotLoaded,
			LoadStatus_NeedsRestart,
		};

		LoadStatus_ load_extension(Path const & path);
		LoadStatus_ reload_extension(Path const & path);
		LoadStatus_ unload_extension(Path const & path);
		bool is_extension_loaded(Path const & path);
		Vector<Path> get_loaded_extensions() const;
		Ref<NativeExtension> get_extension(Path const & path);

		void initialize_extensions(NativeExtension::InitializationLevel_ level);
		void deinitialize_extensions(NativeExtension::InitializationLevel_ level);

		void load_extensions();
	};
}

#endif // !_ISM_NATIVE_EXTENSION_MANAGER_HPP_
