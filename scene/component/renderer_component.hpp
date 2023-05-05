#ifndef _ISM_RENDERER_COMPONENT_HPP_
#define _ISM_RENDERER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Ism
{
	class ISM_API RendererComponent : public Component
	{
		DEFINE_CLASS(RendererComponent, Component);

	public:
		RendererComponent();
		virtual ~RendererComponent() override;
	};
}

#endif // !_ISM_RENDERER_COMPONENT_HPP_
