#ifndef _ISM_EXTENSION_INTERFACE_HPP_
#define _ISM_EXTENSION_INTERFACE_HPP_

#include <core/typedefs.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ExtensionInitializationLevel_
	{
		ExtensionInitializationLevel_Core,
		ExtensionInitializationLevel_Servers,
		ExtensionInitializationLevel_Scene,
		ExtensionInitializationLevel_Editor,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ExtensionInterface
	{
		u32		version_major{};
		u32		version_minor{};
		u32		version_patch{};
		u32		version_build{};
		cstring	version_string{};
		
		// etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ExtensionInitialization
	{
		ExtensionInitializationLevel_ minimum_level{};
		void * user{};
		void(*initialize)(void * user, ExtensionInitializationLevel_ level) {};
		void(*finalize)(void * user, ExtensionInitializationLevel_ level) {};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using ExtensionInitializationFunc = bool(*)(ExtensionInterface const * iface, class Extension * extension, ExtensionInitialization * initialization);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EXTENSION_INTERFACE_HPP_
