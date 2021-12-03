#ifndef _ISM_RENDERER_COMPONENT_HPP_
#define _ISM_RENDERER_COMPONENT_HPP_

#include <scene/components/component.hpp>

namespace ism
{
	class ISM_API RendererComponent : public Component
	{
		OBJECT_COMMON(RendererComponent, Component);

	public:
		RendererComponent(Entity * entity) noexcept;
	};
}

#endif // !_ISM_RENDERER_COMPONENT_HPP_
