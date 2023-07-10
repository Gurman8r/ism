#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering/shader_language.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API RendererCanvasRenderer
	{
	private:
		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

		enum {
			BASE_UNIFORM_SET,
			MATERIAL_UNIFORM_SET,
			TRANSFORMS_UNIFORM_SET,
			CANVAS_TEXTURE_UNIFORM_SET,
		};

		RID m_shader{};
		RID m_pipeline{};

	public:
		RendererCanvasRenderer(RenderingDevice * device, RendererStorage * storage);
		~RendererCanvasRenderer();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
