#ifndef _ISM_CLASS_DETAIL_HPP_
#define _ISM_CLASS_DETAIL_HPP_

#include <core/api/detail/attr.hpp>

namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_string(OBJECT obj, cstring name)
	{
		if (TYPE type; !obj || !name || !(type = typeof(obj)))
		{
			return nullptr;
		}
		else if (GetSetDef * def{ get_property_def(obj, name) }; def && def->get)
		{
			return def->get(obj, def);
		}
		else
		{
			if (isinstance<DICT>(type->tp_dict))
			{
				if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
				{
					return PROPERTY(elem).get(obj);
				}
				else if (elem)
				{
					return elem;
				}
			}

			if (OBJECT * dp{ get_dict_ptr(type, obj) }; dp && isinstance<DICT>(*dp))
			{
				if (OBJECT & elem{ DICT(*dp).get(name) }; isinstance<PROPERTY>(elem))
				{
					return PROPERTY(elem).get(obj);
				}
				else
				{
					return elem;
				}
			}

			return nullptr;
		}
	}

	inline Error impl_setattr_string(OBJECT obj, cstring name, OBJECT value)
	{
		if (TYPE type; !obj || !name || !(type = typeof(obj)))
		{
			return Error_Unknown;
		}
		else if (GetSetDef * def{ get_property_def(obj, name) }; def && def->set)
		{
			return def->set(obj, value, def);
		}
		else if (OBJECT * dp{ get_dict_ptr(type, obj) }; !dp)
		{
			if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
		else if (dp && isinstance<DICT>(*dp))
		{
			if (OBJECT & elem{ DICT(*dp).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
		else
		{
			if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJECT impl_getattr_object(OBJECT obj, OBJECT name)
	{
		if (TYPE type; !obj || !name || !(type = typeof(obj)))
		{
			return nullptr;
		}
		else if (isinstance<STR>(name))
		{
			return impl_getattr_string(obj, STR(name).c_str());
		}
		else
		{
			if (isinstance<DICT>(type->tp_dict))
			{
				if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
				{
					return PROPERTY(elem).get(obj);
				}
				else if (elem)
				{
					return elem;
				}
			}

			if (OBJECT * dp{ get_dict_ptr(type, obj) }; dp && isinstance<DICT>(*dp))
			{
				if (OBJECT & elem{ DICT(*dp).get(name) }; isinstance<PROPERTY>(elem))
				{
					return PROPERTY(elem).get(obj);
				}
				else
				{
					return elem;
				}
			}

			return nullptr;
		}
	}

	inline Error impl_setattr_object(OBJECT obj, OBJECT name, OBJECT value)
	{
		if (TYPE type; !obj || !name || !(type = typeof(obj)))
		{
			return Error_Unknown;
		}
		else if (isinstance<STR>(name))
		{
			return impl_setattr_string(obj, STR(name).c_str(), value);
		}
		else if (OBJECT * dp{ get_dict_ptr(type, obj) }; !dp)
		{
			if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
		else if (dp && isinstance<DICT>(*dp))
		{
			if (OBJECT & elem{ DICT(*dp).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
		else
		{
			if (OBJECT & elem{ DICT(type->tp_dict).get(name) }; isinstance<PROPERTY>(elem))
			{
				return PROPERTY(elem).set(obj, value);
			}
			else
			{
				return (elem = value), Error_None;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLASS_DETAIL_HPP_
