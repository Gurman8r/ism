#ifndef _ISM_COLLIDER_COMPONENT_HPP_
#define _ISM_COLLIDER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace ism
{
	class ISM_API ColliderComponent : public Component
	{
		DEFINE_CLASS(ColliderComponent, Component);

	public:
		ColliderComponent();
		virtual ~ColliderComponent() override;
	};
}

#endif // !_ISM_COLLIDER_COMPONENT_HPP_