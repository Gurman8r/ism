#include <scene/resources/shader_library.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Shader, t) {}

	Shader::~Shader()
	{
		if (m_shader) {
			rendering_device()->shader_destroy(m_shader);
			m_shader = nullptr;
		}
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
