#ifndef _ISM_SHADER_LIBRARY_HPP_
#define _ISM_SHADER_LIBRARY_HPP_

#include <scene/resources/shader.hpp>

namespace ism
{
	class ISM_API ShaderLibrary final
	{
	public:
		static Error_ load_shader(Shader & shader, Path const & path);
		static Error_ load_shader(Ref<Shader> shader, Path const & path);
	};
}

#endif // !_ISM_SHADER_LIBRARY_HPP_