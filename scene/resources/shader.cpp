#include <scene/resources/shader.hpp>
#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

OBJECT_EMBED(Shader, t) {}

Shader::Shader(Path const & path)
{
	set_path(path);

	reload_from_file();
}

Shader::~Shader()
{
	if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); m_shader = nullptr; }
}

Error_ Shader::reload_from_file()
{
	if (!get_path()) { return Error_Unknown; }

	return ShaderLoader::load_shader(*this, get_path());
}
