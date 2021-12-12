#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Shader : public Resource
	{
		OBJECT_COMMON(Shader, Resource);

		mutable RID m_shader{};

	public:
		Shader() noexcept {}

		explicit Shader(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Shader() override;

		NODISCARD virtual RID get_rid() const { return m_shader; }

		virtual void reload_from_file() override;

		void bind();
		void unbind();
		void set_uniform1i(String const & name, int32_t const value);
		void set_uniform1f(String const & name, float_t const value);
		void set_uniform2f(String const & name, Vec2f const & value);
		void set_uniform3f(String const & name, Vec3f const & value);
		void set_uniform4f(String const & name, Vec4f const & value);
		void set_uniform16f(String const & name, Mat4f const & value, bool transpose = false);
	};
}

#endif // !_ISM_SHADER_HPP_
