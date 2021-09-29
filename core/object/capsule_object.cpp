#include <core/object/capsule_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(CapsuleObject, t, TypeFlags_BaseType)
{
	t.tp_bind = CLASS_BINDFUNC(CapsuleObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */