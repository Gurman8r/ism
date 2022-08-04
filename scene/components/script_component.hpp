#ifndef _ISM_SCRIPT_COMPONENT_HPP_
#define _ISM_SCRIPT_COMPONENT_HPP_

#include <scene/components/behavior_component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ScriptComponent : public BehaviorComponent
	{
		OBJECT_CLASS(ScriptComponent, BehaviorComponent);

	protected:
		ScriptComponent(Entity * entity) noexcept;

	public:
		virtual ~ScriptComponent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCRIPT_COMPONENT_HPP_
