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
		static RD::ShaderCreateInfo load_spec(JSON const & json);
	};
}

#endif // !_ISM_SHADER_LOADER_OPENGL_HPP_
#endif // OPENGL_ENABLED
