#include <scene/resources/shader.hpp>

#ifdef ISM_RENDERER_OPENGL
#include <drivers/opengl/opengl_shader.hpp>
#endif

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Shader, t, "shader")
{
}

Shader::~Shader()
{
}

Shader::Shader() : Resource{}
{
}