#ifndef _ISM_SHADER_LOADER_HPP_
#define _ISM_SHADER_LOADER_HPP_

#include <scene/resources/shader.hpp>

namespace ism
{
	class ISM_API ShaderLoader final
	{
	public:
		/* load from file */
		static Error_ load_shader(Shader & shader, Path const & path);
		static Error_ load_shader(Ref<Shader> shader, Path const & path);

		/* load from memory */
		static Error_ load_shader(Shader & shader, DynamicBuffer const & code);
		static Error_ load_shader(Ref<Shader> shader, DynamicBuffer const & code);
	};
}

#endif // !_ISM_SHADER_LOADER_HPP_
