#include <core/object/capsule_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMP(CapsuleObject, t, TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(CapsuleObject); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<CapsuleObject>(type);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */