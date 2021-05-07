#include <core/api/types/bool_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static NumberMethods bool_as_number = COMPOSE(NumberMethods, m)
{
};

//CoreType ism::_CoreBool_Type = COMPOSE(CoreType, t)
//{
//	t.tp_name = "bool";
//	t.tp_basicsize = sizeof(CoreInt);
//	t.tp_base = typeof<INT>();
//	t.tp_operator_delete = (freefunc)[](void * ptr) {};
//};
//
//CoreInt ism::_Core_True = CoreInt{ CoreBool_Type, true };
//
//CoreInt ism::_Core_False = CoreInt{ CoreBool_Type, false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */