#ifndef _ISM_API_DETAIL_CLASS_HPP_
#define _ISM_API_DETAIL_CLASS_HPP_

#include <core/api/detail/attr.hpp>

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_string(OBJECT o, cstring name)
	{
		if (!o.is_valid())
		{
			return nullptr;
		}
		else if (GetSetDef * def{ get_property_def(o, name) })
		{
			return CHECK(def->get)(o, def);
		}
		else if (OBJECT * dict{ get_dict_ptr(o) })
		{
			return getitem(*dict, name);
		}
		else
		{
			return nullptr;
		}
	}

	inline Error impl_setattr_string(OBJECT o, cstring name, OBJECT v)
	{
		if (!o.is_valid())
		{
			return Error_Unknown;
		}
		else if (GetSetDef * def{ get_property_def(o, name) })
		{
			return CHECK(def->set)(o, v, def);
		}
		else if (OBJECT * dict{ get_dict_ptr(o) })
		{
			return setitem(*dict, name, v);
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_object(OBJECT o, OBJECT name)
	{
		if (!o.is_valid())
		{
			return nullptr;
		}
		else if (isinstance<STR>(name))
		{
			return impl_getattr_string(o, STR(name)->c_str());
		}
		else if (OBJECT * dict{ get_dict_ptr(o) })
		{
			return getitem(*dict, name);
		}
		else
		{
			return nullptr;
		}
	}

	inline Error impl_setattr_object(OBJECT o, OBJECT name, OBJECT v)
	{
		if (!o.is_valid())
		{
			return Error_Unknown;
		}
		else if (isinstance<STR>(name))
		{
			return impl_setattr_string(o, STR(name)->c_str(), v);
		}
		else if (OBJECT * dict{ get_dict_ptr(o) })
		{
			return setitem(*dict, name, v);
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_DETAIL_CLASS_HPP_
