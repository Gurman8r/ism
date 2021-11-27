#ifndef _ISM_MODULE_OBJECT_HPP_
#define _ISM_MODULE_OBJECT_HPP_

#include <core/api/object/cppfunction_object.hpp>

// module
namespace ism
{
	// module object
	class ISM_API ModuleObject : public Object
	{
		OBJECT_COMMON(ModuleObject, Object);

		friend class MODULE;

	public:
		DICT m_dict{};

		STR m_name{};

		ModuleObject() noexcept : Object{} {}

		ModuleObject(STR const & name) : Object{}, m_dict{ DICT::new_() }, m_name{ name } {}

		template <class Value = OBJ
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			STR str_name{ name };
			VERIFY(overwrite || !ism::hasattr(this, str_name));
			m_dict[str_name] = FWD(value);
		}

		template <class Func, class ... Extra
		> ModuleObject & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(this),
				attr::sibling(ism::getattr(this, name, nullptr)),
				FWD(extra)... });
			return add_object(name, cf, true), (*this);
		}

		MODULE def_submodule(cstring name);

		void reload();

	public:
		static OBJ module_getattro(MODULE m, OBJ name);
	};

	// module delete
	template <> struct DefaultDelete<ModuleObject> : DefaultDelete<Object> {};

	// module check
#define OBJECT_CHECK_MODULE(o) (ism::isinstance<ism::MODULE>(o))

	// module ref
	class MODULE : public Ref<ModuleObject>
	{
		REF_COMMON(MODULE, OBJECT_CHECK_MODULE);

	public:
		template <class Value = OBJ
		> void add_object(cstring name, Value && value, bool overwrite = false) {
			m_ptr->add_object(name, FWD(value), overwrite);
		}

		template <class Func, class ... Extra
		> MODULE & def(cstring name, Func && func, Extra && ... extra) {
			return m_ptr->def(name, FWD(func), FWD(extra)...), (*this);
		}

		MODULE def_submodule(cstring name) { return m_ptr->def_submodule(name); }

		void reload() { m_ptr->reload(); }
	};
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Name = cstring
	> MODULE create_extension_module(Name && name) noexcept
	{
		DICT modules{ get_internals().modules };

		STR str_name{ FWD_OBJ(name) };

		return !modules.contains(str_name) ? (modules[str_name] = MODULE({ str_name })) : nullptr;
	}

	template <class Name = cstring
	> NODISCARD MODULE import_module(Name && name) noexcept
	{
		DICT modules{ get_internals().modules };

		STR str_name{ FWD_OBJ(name) };

		return modules.lookup(name);
	}

	NODISCARD inline DICT globals() noexcept
	{
		STR_IDENTIFIER(__main__);
		STR_IDENTIFIER(__dict__);

		MODULE m{ import_module(&ID___main__) };
		
		if (!m) { return nullptr; }
		
		return getattr(m, &ID___dict__);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODULE_OBJECT_HPP_
