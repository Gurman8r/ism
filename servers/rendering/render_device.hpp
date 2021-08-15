#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API RenderDevice : public Super
	{
		ISM_SUPER(RenderDevice, Super);

	public:
		virtual ~RenderDevice();
	};

	ISM_API_FUNC(RenderDevice *) get_render_device();

	ISM_API_FUNC(RenderDevice *) set_render_device(RenderDevice * value);
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
