#include <core/object/generic_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPL(GenericObject, t, "generic_type", TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(GenericObject); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<GenericObject>(type);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */