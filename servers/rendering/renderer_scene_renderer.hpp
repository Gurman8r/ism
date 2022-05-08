#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer : public Object
	{
		OBJECT_COMMON(RendererSceneRenderer, Object);

		static RendererSceneRenderer * singleton;

		friend class RenderingServerDefault;

	protected:
		explicit RendererSceneRenderer();

	public:
		virtual ~RendererSceneRenderer();

		FORCE_INLINE static RendererSceneRenderer * get_singleton() noexcept { return singleton; }

	public:
		void render_scene();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SCENE_RENDERER (ism::RendererSceneRenderer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
