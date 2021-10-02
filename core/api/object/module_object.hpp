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
		template <class Func, class ... Extra
		> MODULE & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(*this),
				attr::sibling(ism::getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(name, cf, true), (*this);
		}

		template <class Value = OBJ
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			STR str_name{ name };

			VERIFY(overwrite || !ism::hasattr(*this, str_name));

			m_ptr->m_dict[str_name] = FWD(value);
		}

		MODULE def_submodule(cstring name)
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

	template <class Name = cstring
	> MODULE create_extension_module(Name && name) noexcept
	{
		DICT modules{ get_internals().modules };

		STR str_name{ FWD_OBJ(name) };

		return !modules.contains(str_name) ? (modules[str_name] = MODULE({ str_name })) : nullptr;
	}

	template <class Name = cstring
	> NODISCARD MODULE import(Name && name) noexcept
	{
		DICT modules{ get_internals().modules };

		STR str_name{ FWD_OBJ(name) };

		return modules.lookup(name, MODULE{});
	}

	NODISCARD inline DICT globals() noexcept
	{
		STR_IDENTIFIER(__main__);
		MODULE m{ ism::import(&ID___main__) };
		if (!m) { return nullptr; }
		
		STR_IDENTIFIER(__dict__);
		DICT d{ m.attr(&ID___dict__) };
		if (!d) { return nullptr; }

		return d;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODULE_OBJECT_HPP_
