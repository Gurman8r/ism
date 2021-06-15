#ifndef _ISM_MODULE_OBJECT_HPP_
#define _ISM_MODULE_OBJECT_HPP_

#include <core/api/object/cppfunction_object.hpp>

// module object
class NODISCARD ISM_API ism::api::ModuleObject : public BaseObject
{
	ISM_OBJECT(ModuleObject, BaseObject);

protected:
	static void _bind_class(TypeObject & t);

public:
	DICT		m_dict{ DictObject{} };
	STR			m_name{};
	STR			m_doc{};
	inquiry		m_clear{};
	freefunc	m_free{};

public:
	ModuleObject(cstring name) : self_type{}
	{
		m_name = STR(name);
	}
};

// module deleter
template <> struct ism::DefaultDelete<ism::api::ModuleObject> : ism::DefaultDelete<ism::api::BaseObject> {};

// module handle
template <> class ism::api::Handle<ism::api::ModuleObject> : public ism::api::BaseHandle<ism::api::ModuleObject>
{
	ISM_HANDLE(ModuleObject);
	
public:
	Handle() = default;
	
	~Handle() = default;

	template <class Func, class ... Extra
	> MODULE & def(cstring name, Func && func, Extra && ... extra)
	{
		CPP_FUNCTION cf({
			FWD(func),
			attr::name(name),
			attr::scope(ptr()),
			attr::sibling(attr(name)),
			FWD(extra)... });
		attr(cf.name()) = cf;
		return (*this);
	}

	template <class Name = cstring, class Value = OBJECT
	> void add_object(Name && name, Value && value, bool overwrite = false)
	{
		if (auto i{ FWD_OBJ(name) }; overwrite || !m_ref->m_dict->contains(i))
		{
			m_ref->m_dict[i] = FWD_OBJ(value);
		}
	}

	MODULE def_submodule(cstring name, cstring doc = "")
	{
		return MODULE{};
	}

	void reload()
	{
	}
};

namespace ism::api
{
	ISM_API_FUNC(OBJECT) module_getattr(MODULE m, OBJECT name);

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ object_or_cast(name) };
		return d.contains(i) ? MODULE{} : d[i] = MODULE({ name });
	}

	inline MODULE import(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ object_or_cast(name) };
		return !d.contains(i) ? MODULE{} : d[i];
	}

	inline DICT globals()
	{
		if (auto frame{ get_default_frame() }; frame)
		{
			return CHECK(frame->globals);
		}
		else
		{
			return api::import("__main__").attr("__dict__");
		}
	}
}

#endif // !_ISM_MODULE_OBJECT_HPP_