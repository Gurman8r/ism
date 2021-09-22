#include <core/io/resource.hpp>

using namespace ism;

ISM_OBJECT_IMPL(Resource, t, "resource", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_bind = (bindfunc)[](TYPE type)->TYPE
	{
		return type;
	};
};

Resource::~Resource() {}