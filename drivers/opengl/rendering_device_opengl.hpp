#ifdef OPENGL_ENABLED
#ifndef _ISM_RENDERING_DEVICE_OPENGL_HPP_
#define _ISM_RENDERING_DEVICE_OPENGL_HPP_

#include <drivers/opengl/opengl.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingDeviceOpenGL : public RenderingDevice
	{
		OBJECT_COMMON(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();

		RenderingDeviceOpenGL();

		virtual void initialize() override;

		virtual void finalize() override;

	public:
		virtual void clear(Color const & color) override;

		virtual void draw_arrays(RenderPrimitive primitive, size_t first, size_t count) override;

		virtual void draw_indexed(RenderPrimitive primitive, size_t count) override;

		virtual void flush() override;

		virtual void set_viewport(Rect const & rect) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
