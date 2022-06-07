#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer : public Object
	{
		OBJECT_COMMON(RendererSceneRenderer, Object);

		friend class RenderingServerDefault;

	protected:
		explicit RendererSceneRenderer();

	public:
		virtual ~RendererSceneRenderer();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
