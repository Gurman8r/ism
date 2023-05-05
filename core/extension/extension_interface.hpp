#ifndef _ISM_EXTENSION_INTERFACE_HPP_
#define _ISM_EXTENSION_INTERFACE_HPP_

#include <core/typedefs.hpp>

using IsmExtensionInterfacePtr = void *;
using IsmExtensionPtr = void *;
using IsmExtensionInitializationPtr = void *;
using IsmExtensionInitializationLevel = int;

namespace Ism
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
		void(*initialize)(void * user, IsmExtensionInitializationLevel level) {};
		void(*finalize)(void * user, IsmExtensionInitializationLevel level) {};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using ExtensionInitializationFunc = bool(*)(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr init);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EXTENSION_INTERFACE_HPP_
