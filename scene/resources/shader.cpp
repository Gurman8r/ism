#include <scene/resources/shader.hpp>
#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

OBJECT_EMBED(Shader, t) {}

Shader::~Shader()
{
	if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); m_shader = nullptr; }
}

Error_ Shader::reload_from_file()
{
	return ShaderLoader::load_shader(*this, get_path());
}
