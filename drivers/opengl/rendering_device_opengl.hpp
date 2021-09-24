#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <drivers/opengl/opengl.hpp>

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJ_COMMON(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();

		RenderingDeviceOpenGL();

	private:

	};
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
