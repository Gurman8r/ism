#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	class ISM_API Shader : public Resource
	{
		OBJECT_COMMON(Shader, Resource);

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
		friend class ShaderLoader;

		RID m_shader{};

		Mode_ m_mode{ Mode_MAX };

	public:
		Shader() noexcept {}

		explicit Shader(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Shader() override;

		virtual Error_ reload_from_file() override;

		virtual RID get_rid() const { return m_shader; }

		virtual Mode_ get_mode() const { return m_mode; }

	public:
		String get_code() const;
		void set_code(const String & value);
	};
}

#endif // !_ISM_SHADER_HPP_
