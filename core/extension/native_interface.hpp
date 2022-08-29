#ifndef _ISM_NATIVE_INTERFACE_HPP_
#define _ISM_NATIVE_INTERFACE_HPP_

// PLACEHOLDER

#include <core/typedefs.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum NativeInitializationLevel_
	{
		NativeInitializationLevel_Core,
		NativeInitializationLevel_Servers,
		NativeInitializationLevel_Scene,
		NativeInitializationLevel_Editor,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NativeInterface
	{
		u32		version_major{};
		u32		version_minor{};
		u32		version_patch{};
		u32		version_build{};
		cstring	version_string{};
		
		// etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_NATIVE_INTERFACE_HPP_
