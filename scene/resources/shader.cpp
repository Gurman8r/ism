#include <scene/resources/shader_loader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Shader, t) {}

	Shader::~Shader()
	{
		if (m_shader) { RD::get_singleton()->shader_destroy(m_shader); m_shader = nullptr; }
	}

	Error_ Shader::reload_from_file()
	{
		return ShaderLoader::load_from_file(*this, get_path());
	}

	String Shader::get_code() const
	{
		return {};
	}

	void Shader::set_code(String const & value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
