#include <core/object/capsule_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(CapsuleObject, t, TypeFlags_BaseType)
{
	t.tp_bind = MAKE_CLASS_BINDER(CapsuleObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */