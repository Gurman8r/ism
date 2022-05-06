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

	public:
		void bind();
		void unbind();
		
		void set_uniform1i(String const & name, int32_t const value);
		void set_uniform1f(String const & name, float_t const value);
		void set_uniform2f(String const & name, Vec2f const & value);
		void set_uniform3f(String const & name, Vec3f const & value);
		void set_uniform4f(String const & name, Vec4f const & value);
		void set_uniform16f(String const & name, Mat4f const & value, bool transpose = false);
		void set_uniform_texture(String const & name, RID const value, size_t slot = 0);

		template <class T, class ... Args
		> void set_uniform(String const & name, T const & value, Args && ... args) noexcept
		{
			if constexpr (std::is_floating_point_v<T> || std::is_same_v<T, Duration>) { set_uniform1f(name, (float_t)value); }
			else if constexpr (std::is_same_v<T, RID>) { set_uniform_texture(name, value, FWD(args)...); }
			else if constexpr (std::is_integral_v<T>) { set_uniform1i(name, (int32_t)value); }
			else if constexpr (std::is_same_v<T, Vec2>) { set_uniform2f(name, value); }
			else if constexpr (std::is_same_v<T, Vec3>) { set_uniform3f(name, value); }
			else if constexpr (std::is_same_v<T, Vec4>) { set_uniform4f(name, value); }
			else if constexpr (std::is_same_v<T, Color>) { set_uniform4f(name, value); }
			else if constexpr (std::is_same_v<T, Mat4>) { set_uniform16f(name, value, FWD(args)...); }
			else { static_assert(!"invalid type"); }
		}
	};
}

#endif // !_ISM_SHADER_HPP_
