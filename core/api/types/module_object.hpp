#ifndef _ISM_API_TYPES_MODULE_OBJECT_HPP_
#define _ISM_API_TYPES_MODULE_OBJECT_HPP_

#include <core/api/types/cppfunction_object.hpp>

// module
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreModule, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		DICT		m_dict{ CoreDict{} };
		STR			m_name{};
		STR			m_doc{};
		inquiry		m_clear{};
		freefunc	m_free{};

		CoreModule(cstring name) : self_type{}
		{
			m_name = STR(name);
		}

		template <class Func, class ... Extra
		> CoreModule & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				detail::name(name),
				detail::scope(this),
				detail::sibling(attr(name)),
				FWD(extra)... });
			attr(cf->name()) = cf;
			return (*this);
		}

		MODULE def_submodule(cstring name, cstring doc = "")
		{
			return MODULE{};
		}

		template <class Name = cstring, class O = OBJECT
		> void add_object(Name && name, O && value, bool overwrite = false)
		{
			auto i{ detail::object_forward(FWD(name)) };
			if (m_dict->contains(i) && !overwrite) { return; }
			m_dict[i] = detail::object_forward(FWD(value));
		}

		void reload()
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ detail::object_forward(name) };
		if (d->contains(i)) { return nullptr; }
		else { return d[i] = MODULE({ name }); }
	}

	inline MODULE import_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ detail::object_forward(name) };
		if (!d->contains(i)) { return nullptr; }
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

#endif // !_ISM_API_TYPES_MODULE_OBJECT_HPP_
