#ifndef _ISM_OPENGL_RENDERING_DEVICE_HPP_
#define _ISM_OPENGL_RENDERING_DEVICE_HPP_

#include <drivers/opengl/opengl.hpp>

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	API_CLASS(OpenGL_RenderingDevice) : public RenderingDevice
	{
		OBJ_COMMON(OpenGL_RenderingDevice, RenderingDevice);

	public:
		~OpenGL_RenderingDevice();

		OpenGL_RenderingDevice();

	private:

	};
}

#endif // !_ISM_OPENGL_DEVICE_HPP_
