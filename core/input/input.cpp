#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input * Input::__singleton{};

	OBJECT_EMBED(Input, t)
	{
		t.tp_install = CLASS_INSTALLER(Input, t)
		{
			return t;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}