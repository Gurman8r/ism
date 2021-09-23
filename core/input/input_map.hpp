#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	API_CLASS(InputMap) : public Object
	{
		OBJ_COMMON(InputMap, Object);

	public:
		virtual ~InputMap() override;

		InputMap() noexcept : Object{} {}
	};
}

#endif // !_ISM_INPUT_MAP_HPP_
