#ifndef _ISM_CLASS_DETAIL_HPP_
#define _ISM_CLASS_DETAIL_HPP_

#include <core/api/detail/attr.hpp>

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_string(OBJECT o, cstring i)
	{
		if (TYPE t{ typeof(o) }; !t || !t->tp_dict)
		{
			return nullptr;
		}
		else if (GetSetDef * def{ get_property_def(o, i) }; def && def->get)
		{
			return def->get(o, def);
		}
		else if (DICT d{ *get_dict_ptr(t, o) })
		{
			return d.get(i);
		}
		else
		{
			return nullptr;
		}
	}

	inline Error impl_setattr_string(OBJECT o, cstring i, OBJECT v)
	{
		if (TYPE t{ typeof(o) }; !t || !t->tp_dict)
		{
			return Error_Unknown;
		}
		else if (GetSetDef * def{ get_property_def(o, i) }; def && def->set)
		{
			return def->set(o, v, def);
		}
		else if (DICT d{ *get_dict_ptr(t, o) })
		{
			return d.set(i, v);
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_object(OBJECT o, OBJECT i)
	{
		if (TYPE t{ typeof(o) }; !t || !t->tp_dict)
		{
			return nullptr;
		}
		else if (isinstance<STR>(i))
		{
			return impl_getattr_string(o, STR(i).c_str());
		}
		else if (DICT d{ *get_dict_ptr(t, o) })
		{
			return d.get(i);
		}
		else
		{
			return nullptr;
		}
	}

	inline Error impl_setattr_object(OBJECT o, OBJECT i, OBJECT v)
	{
		if (TYPE t{ typeof(o) }; !t || !t->tp_dict)
		{
			return Error_Unknown;
		}
		else if (isinstance<STR>(i))
		{
			return impl_setattr_string(o, STR(i).c_str(), v);
		}
		else if (DICT d{ *get_dict_ptr(t, o) })
		{
			return d.set(i, v);
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLASS_DETAIL_HPP_
