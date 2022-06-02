#ifndef _ISM_SHADER_HPP_
#define _ISM_SHADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API Shader : public Resource
	{
		OBJECT_COMMON(Shader, Resource);

		friend class ShaderLoader;

		RID m_shader{};

	public:
		Shader() noexcept {}

		explicit Shader(Path const & path) noexcept { set_path(path); reload_from_file(); }

		virtual ~Shader() override;

		virtual Error_ reload_from_file() override;

		NODISCARD virtual RID get_rid() const { return m_shader; }
	};
}

#endif // !_ISM_SHADER_HPP_
