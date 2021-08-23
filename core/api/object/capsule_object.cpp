#include <core/api/object/capsule_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPLEMENTATION(CapsuleObject, t, "capsule", TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(CapsuleObject); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<CAPSULE>(type);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */