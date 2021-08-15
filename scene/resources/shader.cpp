#include <scene/resources/shader.hpp>

#ifdef ISM_RENDERER_OPENGL
#include <drivers/opengl/opengl_shader.hpp>
#endif

namespace ism
{
	Shader::~Shader()
	{
	}

	Shader::Shader()
	{
	}
}