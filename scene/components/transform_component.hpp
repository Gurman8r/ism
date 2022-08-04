#ifndef _ISM_TRANSFORM_COMPONENT_HPP_
#define _ISM_TRANSFORM_COMPONENT_HPP_

#include <scene/components/component.hpp>

namespace ism
{
	class ISM_API TransformComponent : public Component
	{
		OBJECT_CLASS(TransformComponent, Component);

	public:
		TransformComponent(Entity * entity) noexcept;
	};
}

#endif // !_ISM_TRANSFORM_COMPONENT_HPP_
