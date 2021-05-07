#ifndef _ISM_API_TYPES_GENERIC_OBJECT_HPP_
#define _ISM_API_TYPES_GENERIC_OBJECT_HPP_

#include <core/api/types/module_object.hpp>

// generic
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreGeneric : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreGeneric, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		DICT m_dict{ CoreDict{} };
		vectorcallfunc m_vectorcall{};

	protected:
		void def_property_static_impl(cstring name, OBJECT fget, OBJECT fset, detail::function_record * rec_func)
		{
			//bool const is_static{ rec_func && !(rec_func->is_method && rec_func->scope) };
			//bool const has_doc{ rec_func && !rec_func->doc.empty() };
			//OBJECT property{ is_static ? typeof<STATIC_PROPERTY>() : typeof<PROPERTY>() };
			attr(name) = PROPERTY({ fget, fset, nullptr });
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_GENERIC_OBJECT_HPP_
