#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	class ISM_API RendererSceneRenderer : public Object
	{
		OBJECT_COMMON(RendererSceneRenderer, Object);

	public:
		RendererSceneRenderer();

		virtual ~RendererSceneRenderer();
	};
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
