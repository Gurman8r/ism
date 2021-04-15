#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreModule_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreModule);
	t.tp_name = "module";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return MODULE::create(); };
	
	t.tp_dictoffset = offsetof(CoreModule, m_dict);

	t.tp_getattr = (getattrfunc)_get_attr_string;
	t.tp_setattr = (setattrfunc)_set_attr_string;

	t.tp_compare = (cmpfunc)[](MODULE o, OBJECT v)
	{
		if (isinstance<MODULE>(v))
		{
			return (o.ptr() == v.ptr()) ? 0 : util::compare(o->m_name, MODULE(v)->m_name);
		}
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };

	t.tp_getsets =
	{
		GetSetDef{ "__dict__", (getter)[](MODULE o, auto) { return o->m_dict; }, },
		GetSetDef{ "__name__", (getter)[](MODULE o, auto) { return o->m_name; }, },
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */