#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <servers/rendering_server.hpp>

namespace Ism
{
	class ISM_API Shader : public Resource
	{
		DEFINE_CLASS(Shader, Resource);

		friend class ShaderFormatLoader;

	public:
		enum Mode_
		{
			Mode_CanvasItem,
			Mode_Spatial,
			Mode_Particles,
			Mode_Sky,
			Mode_MAX
		};

	private:
		RID m_shader{};
		Mode_ m_mode{ Mode_MAX };

	public:
		Shader() noexcept = default;
		virtual ~Shader() override;
		virtual RID get_rid() const { return m_shader; }
		virtual Mode_ get_mode() const { return m_mode; }

	public:
		String get_code() const;
		void set_code(String const & value);
	};
}

#endif // !_ISM_SHADER_HPP_
