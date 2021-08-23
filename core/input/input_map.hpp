#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API InputMap : public Object
	{
		ISM_OBJECT_DEFAULT(InputMap, Object);

	public:
		virtual ~InputMap() override;

		InputMap() noexcept : InputMap{ get_class() } {}
	};
}

#endif // !_ISM_INPUT_MAP_HPP_
