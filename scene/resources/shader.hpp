#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <servers/rendering_server.hpp>

namespace Ism
{
	class ISM_API Shader : public Resource
	{
		OBJECT_CLASS(Shader, Resource);

		friend class ShaderFormatLoader;

	private:
		RID m_shader{};
		RS::ShaderMode_ m_mode{ RS::ShaderMode_MAX };

	public:
		Shader() noexcept = default;
		virtual ~Shader() override;

		virtual RID get_rid() const { return m_shader; }
		virtual RS::ShaderMode_ get_mode() const { return m_mode; }

		String get_code() const;
		void set_code(String const & value);
	};
}

#endif // !_ISM_SHADER_HPP_
