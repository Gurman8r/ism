#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <core/api/class.hpp>

namespace ism
{
	// rendering device
	class ISM_API RenderingDevice : public Object
	{
		ISM_OBJECT_DEFAULT(RenderingDevice, Object);

	public:
		virtual ~RenderingDevice();
	};

	ISM_API_FUNC(RenderingDevice *) get_render_device();

	ISM_API_FUNC(RenderingDevice *) set_render_device(RenderingDevice * value);
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
