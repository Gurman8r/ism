#if OPENGL_ENABLED
#ifndef _ISM_SHADER_LOADER_OPENGL_HPP_
#define _ISM_SHADER_LOADER_OPENGL_HPP_

#include <servers/rendering/rendering_device.hpp>
#include <scene/resources/shader.hpp>

namespace ism
{
	class ISM_API ShaderLoaderGLSL final
	{
	public:
		static Error_ generate_glsl(RD::ShaderStageData(&spec)[RD::ShaderStage_MAX], JSON const & json);
	};
}

#endif // !_ISM_SHADER_LOADER_OPENGL_HPP_
#endif // OPENGL_ENABLED
