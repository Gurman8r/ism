#ifndef _ISM_MODULE_OBJECT_HPP_
#define _ISM_MODULE_OBJECT_HPP_

#include <core/api/object/cppfunction_object.hpp>
#include <core/api/runtime.hpp>

// module
namespace ism
{
	// module object
	class ISM_API ModuleObject : public BaseObject
	{
		ISM_OBJECT_TYPED(ModuleObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		DICT		m_dict	{ DictObject{} };
		STR			m_name	{};
		STR			m_doc	{};
		inquiry		m_clear	{};
		freefunc	m_free	{};

		ModuleObject(String const & name) : base_type{ get_type_static() } { m_name = name; }
	};

	// module delete
	template <> struct DefaultDelete<ModuleObject> : DefaultDelete<BaseObject> {};

	// module check
#define ISM_MODULE_CHECK(o) (ism::isinstance<MODULE>(o))

	// module handle
	template <> class Handle<ModuleObject> : public BaseHandle<ModuleObject>
	{
		ISM_HANDLE_DEFAULT(ModuleObject, ISM_MODULE_CHECK);

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
			return add_object(name, cf, true);
		}

		template <class Name = cstring, class Value = OBJECT
		> MODULE & add_object(Name && name, Value && value, bool overwrite = false)
		{
			VERIFY(overwrite || !hasattr(*this, name));
			m_ptr->m_dict[name] = value;
			return (*this);
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

	ISM_API_FUNC(OBJECT) module_getattro(MODULE m, OBJECT name);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_interpreter_state()->modules };
		auto i{ object_or_cast(name) };
		return d.contains(i) ? MODULE{} : d[i] = MODULE({ name });
	}

	inline MODULE import_module(cstring name)
	{
		DICT d{ get_interpreter_state()->modules };
		auto i{ object_or_cast(name) };
		return !d.contains(i) ? MODULE{} : d[i];
	}

	inline DICT globals()
	{
		if (StackFrame * frame{ get_current_frame() })
		{
			return CHECK(frame->globals);
		}
		else
		{
			return import_module("__main__").attr("__dict__");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODULE_OBJECT_HPP_
