#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/input_event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Input : public Super
	{
		ISM_SUPER(Input, Super);

	private:
		static Input * singleton;

	public:
		virtual ~Input();

		DEFAULT_COPYABLE_MOVABLE(Input);

	public:
		FORCE_INLINE static Input * get_singleton() { return singleton; }

		NODISCARD bool get_key_down(int32_t) const { return false; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Input & get_input() { return *CHECK(Input::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
