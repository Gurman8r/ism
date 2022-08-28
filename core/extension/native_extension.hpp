#ifndef _ISM_NATIVE_EXTENSION_HPP_
#define _ISM_NATIVE_EXTENSION_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class NativeExtensionManager;

	class ISM_API NativeExtension : public Resource
	{
		OBJECT_CLASS(NativeExtension, Resource);

	public:
		NativeExtension();
		~NativeExtension();
	};
}

#endif // !_ISM_NATIVE_EXTENSION_HPP_
