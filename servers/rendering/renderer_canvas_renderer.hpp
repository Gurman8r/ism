#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API RendererCanvasRenderer
	{
	private:
		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

		struct SceneState
		{
			struct UBO
			{
			}
			ubo{};
		}
		m_scene_state{};

	public:
		RendererCanvasRenderer(RenderingDevice * device, RendererStorage * storage);
		~RendererCanvasRenderer();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
