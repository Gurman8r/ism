#ifndef _ISM_RIGIDBODY2D_COMPONENT_HPP_
#define _ISM_RIGIDBODY2D_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Ism
{
	class ISM_API Rigidbody2DComponent : public Component
	{
		OBJECT_CLASS(Rigidbody2DComponent, Component);

	public:
		Rigidbody2DComponent();
		virtual ~Rigidbody2DComponent() override;
	};
}

#endif // !_ISM_RIGIDBODY2D_COMPONENT_HPP_
