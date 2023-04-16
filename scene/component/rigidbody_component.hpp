#ifndef _ISM_RIGIDBODY_COMPONENT_HPP_
#define _ISM_RIGIDBODY_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace ism
{
	class ISM_API RigidbodyComponent : public Component
	{
		DEFINE_CLASS(RigidbodyComponent, Component);

	public:
		RigidbodyComponent();
		virtual ~RigidbodyComponent() override;
	};
}

#endif // !_ISM_RIGIDBODY_COMPONENT_HPP_
