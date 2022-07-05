#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MEMBER_IMPL(Input::g_singleton) {};

	OBJECT_EMBED(Input, t)
	{
		t.tp_install = CLASS_INSTALLER(Input, t)
		{
			return t;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}