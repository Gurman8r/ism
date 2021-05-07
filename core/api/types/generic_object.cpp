#include <core/api/types/generic_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreGeneric::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreGeneric::_bind_class(CoreType & t)
{
	t.tp_name = "generic_type";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_base = typeof<TYPE>();

	t.tp_dict_offset = offsetof(CoreGeneric, m_dict);
	t.tp_vectorcall_offset = offsetof(CoreGeneric, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreGeneric *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */