#include <core/api/object/generic_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreGeneric::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "generic_type";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(CoreGeneric, m_dict);
	t.tp_vectorcall_offset = offsetof(CoreGeneric, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreGeneric *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreGeneric::_bind_methods(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */