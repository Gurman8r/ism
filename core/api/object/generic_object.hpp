#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

#include <core/api/object/module_object.hpp>

// generic object
class ISM_API ism::api::GenericObject : public BaseObject
{
	ISM_OBJECT(GenericObject, BaseObject);

protected:
	static void _bind_class(TypeObject & t);

protected:
	void def_property_static_impl(cstring name, OBJECT fget, OBJECT fset, api::function_record * rec_func)
	{
		//bool const is_static{ rec_func && !(rec_func->is_method && rec_func->scope) };
		//bool const has_doc{ rec_func && !rec_func->doc.empty() };
		//OBJECT property{ is_static ? typeof<STATIC_PROPERTY>() : typeof<PROPERTY>() };
		//attr(name) = PROPERTY({ fget, fset, nullptr });
	}

public:
	DICT m_dict{ DictObject{} };

	vectorcallfunc m_vectorcall{};
};

// generic deleter
template <> struct ism::DefaultDelete<ism::api::GenericObject> : ism::DefaultDelete<ism::api::BaseObject> {};

// generic handle
template <> class ism::api::Handle<ism::api::GenericObject> : public ism::api::BaseHandle<ism::api::GenericObject>
{
	ISM_HANDLE(GenericObject);

public:
	Handle() = default;

	~Handle() = default;
};

#endif // !_ISM_GENERIC_OBJECT_HPP_
