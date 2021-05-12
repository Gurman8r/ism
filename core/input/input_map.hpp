#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API InputMap : public Super
	{
		ISM_SUPER_CLASS(InputMap, Super);

	public:
		virtual ~InputMap() override;

		InputMap() {}
	};
}

#endif // !_ISM_INPUT_MAP_HPP_
