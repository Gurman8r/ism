#ifndef _ISM_OPENGL_DEVICE_HPP_
#define _ISM_OPENGL_DEVICE_HPP_

#include <drivers/opengl/opengl.hpp>

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	class ISM_API OpenGL_Device : public RenderingDevice
	{
	public:
		~OpenGL_Device();

		OpenGL_Device();

	private:

	};
}

#endif // !_ISM_OPENGL_DEVICE_HPP_
