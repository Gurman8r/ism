#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	// rendering device
	API_CLASS(RenderingDevice) : public Object
	{
		OBJ_COMMON(RenderingDevice, Object);

		static RenderingDevice * singleton;

	protected:
		explicit RenderingDevice() noexcept : Object{} { singleton = this; }

	public:
		virtual ~RenderingDevice();
	};
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
