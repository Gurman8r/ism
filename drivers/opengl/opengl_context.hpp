#ifndef _ISM_OPENGL_CONTEXT_HPP_
#define _ISM_OPENGL_CONTEXT_HPP_

#include <drivers/opengl/opengl.hpp>

#include <servers/rendering/rendering_context.hpp>

namespace ism
{
	class ISM_API OpenGL_Context : public RenderingContext
	{
	public:
		~OpenGL_Context();

		OpenGL_Context();

	private:

	};
}

#endif // !_ISM_OPENGL_CONTEXT_HPP_
