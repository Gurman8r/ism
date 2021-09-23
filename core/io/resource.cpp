#include <core/io/resource.hpp>

using namespace ism;

OBJ_IMPL(Resource, t, "resource", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_bind = (bindfunc)[](TYPE type)->TYPE
	{
		return type;
	};
};

Resource::~Resource() {}