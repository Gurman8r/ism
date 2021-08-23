#ifndef _ISM_MODULE_OBJECT_HPP_
#define _ISM_MODULE_OBJECT_HPP_

#include <core/api/object/cppfunction_object.hpp>

// module
namespace ism
{
	// module object
	class ISM_API ModuleObject : public Object
	{
		ISM_OBJECT(ModuleObject, Object);

	public:
		DICT		m_dict	{ DICT::new_() };
		STR			m_name	{};
		inquiry		m_clear	{};
		freefunc	m_free	{};

		ModuleObject(String const & name) : Object{ get_class_static() }
		{
			m_name = name;
		}
	};

	// module delete
	template <> struct DefaultDelete<ModuleObject> : DefaultDelete<Object> {};

	// module check
#define ISM_CHECK_MODULE(o) (ism::isinstance<ism::MODULE>(o))

	// module handle
	template <> class Handle<ModuleObject> : public Ref<ModuleObject>
	{
		ISM_HANDLE(ModuleObject, ISM_CHECK_MODULE);

	public:
		template <class Func, class ... Extra
		> MODULE & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(*this),
				attr::sibling(getattr(*this, name, nullptr)),
				FWD(extra)... });
			add_object(name, cf, true);
			return (*this);
		}

		template <class Name = cstring, class Value = OBJ
		> void add_object(Name && name, Value && value, bool overwrite = false)
		{
			VERIFY(overwrite || !hasattr(*this, name));

			m_ptr->m_dict[FWD(name)] = FWD(value);
		}

		MODULE def_submodule(cstring name, cstring doc = "")
		{
			return nullptr;
		}

		void reload()
		{
		}
	};
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) module_getattro(MODULE m, OBJ name);

	ISM_API_FUNC(MODULE) create_extension_module(cstring name);

	ISM_API_FUNC(MODULE) import_module(cstring name);

	inline DICT globals() { return import_module("__main__").attr("__dict__"); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODULE_OBJECT_HPP_
