#ifdef OPENGL_ENABLED

#include <drivers/opengl/rendering_device_opengl.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

EMBEDDED_CLASS_TYPE(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{}
{
	VERIFY(OPENGL_INIT());
}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::initialize()
{
}

void RenderingDeviceOpenGL::finalize()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::clear(Color const & color)
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingDeviceOpenGL::draw_arrays(RenderPrimitive primitive, size_t first, size_t count)
{
	glDrawArrays((uint32_t)primitive, (uint32_t)first, (uint32_t)count);
}

void RenderingDeviceOpenGL::draw_indexed(RenderPrimitive primitive, size_t count)
{
	glDrawElements((uint32_t)primitive, (uint32_t)count, GL_UNSIGNED_INT, nullptr);
}

void RenderingDeviceOpenGL::flush()
{
	glFlush();
}

void RenderingDeviceOpenGL::set_viewport(Rect const & rect)
{
	glViewport((int32_t)rect[0], (int32_t)rect[1], (int32_t)rect[2], (int32_t)rect[3]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
