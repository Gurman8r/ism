#ifndef _ISM_NATIVE_EXTENSION_MANAGER_HPP_
#define _ISM_NATIVE_EXTENSION_MANAGER_HPP_

// PLACEHOLDER

#include <core/extension/native_extension.hpp>

namespace ism
{
	class ISM_API NativeExtensionManager : public Object
	{
		OBJECT_CLASS(NativeExtensionManager, Object);

	public:
		NativeExtensionManager();
		~NativeExtensionManager();
	};
}

#endif // !_ISM_NATIVE_EXTENSION_MANAGER_HPP_
