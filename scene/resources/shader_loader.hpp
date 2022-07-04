#ifndef _ISM_SHADER_LOADER_HPP_
#define _ISM_SHADER_LOADER_HPP_

#include <scene/resources/shader.hpp>
#include <core/io/json.hpp>

namespace ism
{
	class ISM_API ShaderLoader final
	{
	public:
		/* load from file */
		static Error_ load_from_file(Shader & shader, Path const & path);
		static Error_ load_from_file(Ref<Shader> shader, Path const & path);

		/* load from json */
		static Error_ load_from_json(Shader & shader, Json const & json);
		static Error_ load_from_json(Ref<Shader> shader, Json const & json);
	};
}

#endif // !_ISM_SHADER_LOADER_HPP_
