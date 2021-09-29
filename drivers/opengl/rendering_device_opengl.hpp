#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <runtime/renderer/rendering_device.hpp>

namespace ism
{
	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJECT_COMMON(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();

		RenderingDeviceOpenGL();

	public: /* MISC */

	};
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
