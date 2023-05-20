#include <scene/resources/shader_library.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Shader, t) {}

	Shader::~Shader()
	{
		if (m_shader) { get_gpu()->shader_destroy(m_shader); m_shader = nullptr; }
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
