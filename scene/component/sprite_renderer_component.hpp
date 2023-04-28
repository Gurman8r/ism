#ifndef _ISM_SPRITE_RENDERER_COMPONENT_HPP_
#define _ISM_SPRITE_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace ism
{
	class ISM_API SpriteRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(SpriteRendererComponent, RendererComponent);

	public:
		SpriteRendererComponent();
		virtual ~SpriteRendererComponent() override;
	};
}

#endif // !_ISM_SPRITE_RENDERER_COMPONENT_HPP_