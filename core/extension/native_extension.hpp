#ifndef _ISM_NATIVE_EXTENSION_HPP_
#define _ISM_NATIVE_EXTENSION_HPP_

#include <core/extension/native_interface.hpp>

#include <core/io/resource_loader.hpp>

namespace ism
{
	class ISM_API NativeExtension : public Resource
	{
		OBJECT_CLASS(NativeExtension, Resource);

	public:
		NativeExtension();
		~NativeExtension();

		Error_ open_library(Path const & path, String const & entry_symbol);
		void close_library();
		bool is_library_open() const;

		enum InitializationLevel_
		{
			InitializationLevel_Core = NativeInitializationLevel_Core,
			InitializationLevel_Servers = NativeInitializationLevel_Servers,
			InitializationLevel_Scene = NativeInitializationLevel_Scene,
			InitializationLevel_Editor = NativeInitializationLevel_Editor,
		};

		InitializationLevel_ get_minimum_library_initialization_level() const;
		void initialize_library(InitializationLevel_ level);
		void deinitialize_library(InitializationLevel_ level);
	};
}

#endif // !_ISM_NATIVE_EXTENSION_HPP_
