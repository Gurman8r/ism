#ifndef _ISM_SHADER_LIBRARY_HPP_
#define _ISM_SHADER_LIBRARY_HPP_

#include <scene/resources/shader.hpp>

namespace Ism
{
	class ISM_API ShaderLibrary final
	{
	public:
		static Error_ load_shader(Shader & shader, String const & path);
		static Error_ load_shader(Ref<Shader> shader, String const & path);
	};
}

#endif // !_ISM_SHADER_LIBRARY_HPP_
