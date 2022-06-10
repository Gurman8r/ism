#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer : public Object
	{
		OBJECT_COMMON(RendererSceneRenderer, Object);

		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage);

		virtual ~RendererSceneRenderer() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
