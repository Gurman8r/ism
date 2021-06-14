#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

#include <core/api/modsupport.hpp>

namespace ism
{
	class ISM_API InputMap : public Super
	{
		ISM_SUPER(InputMap, Super);

	public:
		virtual ~InputMap() override;

		DEFAULT_COPYABLE_MOVABLE(InputMap);

	public:
	};
}

#endif // !_ISM_INPUT_MAP_HPP_
