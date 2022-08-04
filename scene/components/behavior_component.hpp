#ifndef _ISM_BEHAVIOR_COMPONENT_HPP_
#define _ISM_BEHAVIOR_COMPONENT_HPP_

#include <scene/components/component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API BehaviorComponent : public Component
	{
		OBJECT_CLASS(BehaviorComponent, Component);

		bool m_is_enabled{ true };

	protected:
		BehaviorComponent(Entity * entity) noexcept;

	public:
		virtual ~BehaviorComponent() noexcept override;

		bool is_enabled() const noexcept { return m_is_enabled; }

		void set_enabled(bool value) noexcept { m_is_enabled = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BEHAVIOR_COMPONENT_HPP_
