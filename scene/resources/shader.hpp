#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <servers/rendering_server.hpp>

namespace Ism
{
	enum ShaderMode_
	{
		ShaderMode_CanvasItem,
		ShaderMode_Spatial,
		ShaderMode_Particles,
		ShaderMode_Sky,
		ShaderMode_MAX
	};

	class ISM_API Shader : public Resource
	{
		OBJECT_CLASS(Shader, Resource);

		friend class ShaderFormatLoader;

	private:
		RID m_shader{};
		ShaderMode_ m_mode{ ShaderMode_MAX };

	public:
		Shader() noexcept = default;
		virtual ~Shader() override;
		virtual RID get_rid() const { return m_shader; }
		virtual ShaderMode_ get_mode() const { return m_mode; }

	public:
		String get_code() const;
		void set_code(String const & value);
	};
}

#endif // !_ISM_SHADER_HPP_
