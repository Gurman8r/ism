#ifndef _ISM_TRANSFORM_COMPONENT_HPP_
#define _ISM_TRANSFORM_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Ism
{
	class ISM_API TransformComponent : public Component
	{
		DEFINE_CLASS(TransformComponent, Component);

	public:
		TransformComponent();
		virtual ~TransformComponent() override;
	};
}

#endif // !_ISM_TRANSFORM_COMPONENT_HPP_
