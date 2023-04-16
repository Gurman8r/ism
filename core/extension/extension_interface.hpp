#ifndef _ISM_EXTENSION_INTERFACE_HPP_
#define _ISM_EXTENSION_INTERFACE_HPP_

#include <core/typedefs.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Extension;

	enum ExtensionInitializationLevel_
	{
		ExtensionInitializationLevel_Core,
		ExtensionInitializationLevel_Servers,
		ExtensionInitializationLevel_Scene,
		ExtensionInitializationLevel_Editor,
	};

	struct ExtensionInterface
	{
		u32		version_major{};
		u32		version_minor{};
		u32		version_patch{};
		u32		version_build{};
		cstring	version_string{};
		
		// etc...
	};

	struct ExtensionInitialization
	{
		ExtensionInitializationLevel_ minimum_level{};
		void * userdata{};
		void(*initialize)(void * userdata, ExtensionInitializationLevel_ level);
		void(*finalize)(void * userdata, ExtensionInitializationLevel_ level);
	};

	using ExtensionInitializationFunction = bool(*)(ExtensionInterface const * iface, Extension * library, ExtensionInitialization * init);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EXTENSION_INTERFACE_HPP_
