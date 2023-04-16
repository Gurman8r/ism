#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

// PLACEHOLDER

#include <core/input/input_event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input map
	class ISM_API InputMap : public Object
	{
		DEFINE_CLASS(InputMap, Object);

		friend class Input;

	public:
		InputMap() noexcept;

		virtual ~InputMap() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_MAP_HPP_
