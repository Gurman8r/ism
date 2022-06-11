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

		/* load from json */
		static Error_ load_from_json(Shader & shader, JSON const & json);
		static Error_ load_from_json(Ref<Shader> shader, JSON const & json);

		/* generate source */
		static Error_ generate_glsl(RD::ShaderStageData (&spec)[RD::ShaderStage_MAX], JSON const & json);
		static Error_ generate_hlsl(RD::ShaderStageData (&spec)[RD::ShaderStage_MAX], JSON const & json);
	};
}

#endif // !_ISM_SHADER_LOADER_HPP_
