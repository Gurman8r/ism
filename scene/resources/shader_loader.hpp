#ifndef _ISM_SHADER_LOADER_HPP_
#define _ISM_SHADER_LOADER_HPP_

#include <scene/resources/shader.hpp>

namespace ism
{
	class ISM_API ShaderLoader final
	{
	public:
		/* load from file */
		static Error_ load_from_file(Shader & shader, Path const & path);
		static Error_ load_from_file(Ref<Shader> shader, Path const & path);
	};
}

#endif // !_ISM_SHADER_LOADER_HPP_
