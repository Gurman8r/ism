#ifndef _ISM_SCRIPT_BEHAVIOR_COMPONENT_HPP_
#define _ISM_SCRIPT_BEHAVIOR_COMPONENT_HPP_

#include <scene/components/behavior_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ScriptBehaviorComponent : public BehaviorComponent
	{
		OBJECT_COMMON(ScriptBehaviorComponent, BehaviorComponent);

	protected:
		ScriptBehaviorComponent(Entity * entity) noexcept;

	public:
		virtual ~ScriptBehaviorComponent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCRIPT_BEHAVIOR_COMPONENT_HPP_
