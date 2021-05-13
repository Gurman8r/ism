#ifndef _ISM_MODULE_OBJECT_HPP_
#define _ISM_MODULE_OBJECT_HPP_

#include <core/api/object/cppfunction_object.hpp>

// module
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
		ISM_OBJECT(CoreModule, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		DICT		m_dict{ CoreDict{} };
		STR			m_name{};
		STR			m_doc{};
		inquiry		m_clear{};
		freefunc	m_free{};

	public:
		CoreModule(cstring name) : self_type{}
		{
			m_name = STR(name);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreModule> : public BaseHandle<CoreModule>
	{
		ISM_HANDLE(CoreModule);
	
	public:
		Handle() = default;
	
		~Handle() = default;

		template <class Func, class ... Extra
		> MODULE & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				detail::name(name),
				detail::scope(ptr()),
				detail::sibling(attr(name)),
				FWD(extra)... });
			attr(cf->name()) = cf;
			return (*this);
		}

		template <class Name = cstring, class O = OBJECT
		> void add_object(Name && name, O && value, bool overwrite = false)
		{
			if (auto i{ object_or_cast(FWD(name)) }; overwrite || !m_ref->m_dict->contains(i))
			{
				m_ref->m_dict[i] = object_or_cast(FWD(value));
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ object_or_cast(name) };
		if (d.contains(i)) { return nullptr; }
		else { return d[i] = MODULE({ name }); }
	}

	inline MODULE import_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ object_or_cast(name) };
		if (!d.contains(i)) { return nullptr; }
		else { return d[i]; }
	}

	inline DICT globals()
	{
		if (auto frame{ get_default_frame() }; frame)
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
