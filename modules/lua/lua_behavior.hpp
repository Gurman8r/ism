#ifndef _ISM_LUA_BEHAVIOR_HPP_
#define _ISM_LUA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace ism
{
	// lua behavior
	class ISM_MOD_API LuaBehavior : public Behavior
	{
		DEFINE_CLASS(LuaBehavior, Behavior);

	public:
		LuaBehavior();
		virtual ~LuaBehavior() override;
	};
}

#endif // !_ISM_LUA_BEHAVIOR_HPP_
