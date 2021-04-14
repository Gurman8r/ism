#ifndef _ISM_BIND_HPP_
#define _ISM_BIND_HPP_

#include <core/api/init.hpp>

// cpp_function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCppFunction : public CoreFunction
	{
	public:
		using base_type = CoreFunction;
		using self_type = CoreCppFunction;
		using base_type::type_static;

	public:
		virtual ~CoreCppFunction() override = default;
		
		CoreCppFunction() : base_type{} {}
		
		CoreCppFunction(nullptr_t) : base_type{} {}

		template <class R, class ... Args, class ... Extra
		> CoreCppFunction(R(*f)(Args...), Extra && ... extra)
		{
			this->initialize(f, f, FWD(extra)...);
		}

		template <class Fn, class ... Extra, class = std::enable_if_t<is_lambda_v<Fn>>
		> CoreCppFunction(Fn && f, Extra && ... extra)
		{
			this->initialize(FWD(f), (function_signature_t<Fn> *)nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...), Extra && ... extra)
		{
			this->initialize([f](C * c, Args ... args) -> R {
				return (c->*f)(FWD(args)...);
			}, (R(*)(C *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) &, Extra && ... extra)
		{
			this->initialize([f](C * c, Args ... args) -> R {
				return (c->*f)(FWD(args)...);
			}, (R(*)(C *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) const, Extra && ... extra)
		{
			this->initialize([f](C const * c, Args ... args) -> R {
				return (c->*f)(FWD(args)...);
			}, (R(*)(C const *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) const &, Extra && ... extra)
		{
			this->initialize([f](C const * c, Args ... args) -> R {
				return (c->*f)(FWD(args)...);
			}, (R(*)(C const *, Args...))nullptr, FWD(extra)...);
		}

		NODISCARD OBJECT name() const { return attr("__name__"); }

	protected:
		template <class Fn, class R, class ... Args, class ... Extra
		> void initialize(Fn && f, R(*)(Args...), Extra && ... extra)
		{
			constexpr size_t nargs{ sizeof...(Args) };

			struct Capture { std::remove_reference_t<Fn> f; };

			auto rec{ memnew(FunctionRecord) };

			if (sizeof(Capture) <= sizeof(rec->data))
			{
				::new((Capture *)&rec->data) Capture{ FWD(f) };

				if (!std::is_trivially_destructible_v<Fn>)
				{
					rec->free_data = [](FunctionRecord * r) { ((Capture *)&r->data)->~Capture(); };
				}
			}
			else
			{
				rec->data[0] = memnew(Capture{ FWD(f) });

				rec->free_data = [](FunctionRecord * r) { memdelete((Capture *)r->data[0]); };
			}

			using cast_in = ArgumentLoader<Args...>;
			using cast_out = make_caster<std::conditional_t<std::is_void_v<R>, void_type, R>>;

			rec->impl = [](FunctionCall & call) -> OBJECT
			{
				cast_in args_convt{};
				if (!args_convt.load_args(call)) { return TRY_NEXT_OVERLOAD; }

				// precall

				auto data{ (sizeof(Capture) <= sizeof(call.func.data) ? &call.func.data : call.func.data[0]) };

				auto cap{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnPolicy policy{ call.func.policy };

				OBJECT result{ cast_out::cast(std::move(args_convt).template call<R>(cap->f), policy, call.parent) };

				// postcall

				return result;
			};

			Array<std::type_info const *, nargs> types{};
			mpl::for_types_i<Args...>([&](size_t i, auto tag) noexcept
			{
				types[i] = &typeid(decltype(tag)::type);
			});

			initialize_generic(rec, "signature", types, nargs);
		}

		void initialize_generic(FunctionRecord * rec, cstring text, std::type_info const * const * types, size_t nargs)
		{
		}

		static void destruct(FunctionRecord * rec)
		{
			while (rec)
			{
				FunctionRecord * next{ rec->next };
				if (rec->free_data) { rec->free_data(rec); }
				memdelete_nonzero(rec->def);
				memdelete(rec);
				rec = next;
			}
		}

		static OBJECT dispatcher(CAPSULE self, LIST args_in)
		{
			FunctionRecord const
				* overloads{ (FunctionRecord *)self->get_pointer() },
				* it{ overloads };

			OBJECT
				parent{ !args_in->empty() ? args_in->front() : nullptr },
				result{ TRY_NEXT_OVERLOAD };

			for (; it != nullptr; it = it->next)
			{
				FunctionRecord const & func{ *it };

				FunctionCall call{ func, parent };

				result = func.impl(call);
			}

			return result;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// module
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ModuleDef
	{
		cstring name, doc;

		ssize_t size;

		Vector<MethodDef> methods;

		inquiry clear;

		freefunc free;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreModule_Type;
#define CoreModule_Type		(ism::TYPE{ &_CoreModule_Type })

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreModule;

		NODISCARD static auto type_static() { return CoreModule_Type; }

		struct storage_type
		{
			DICT	dict;
			STR		name;
			void *	state;
		}
		m_data;

	public:
		virtual ~CoreModule() override = default;
		CoreModule() : base_type{ type_static() }, m_data{} {}
		CoreModule(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreModule(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		CoreModule(self_type const &) = default;
		CoreModule(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		template <class O
		> CoreModule(ObjectAPI<O> const & o) : self_type{}
		{
		}

		CoreModule(cstring name) : self_type{ storage_type{ DICT::create(), STR::create(name) } }
		{
		}

		template <class F, class ... Extra
		> self_type & def(cstring name, F && f, Extra && ... extra)
		{
			auto func{ CPP_FUNCTION::create(
				FWD(f),
				_Name{ name },
				_Scope{ handle() },
				_Sibling{ ism::getattr(handle(), name, nullptr) },
				FWD(extra)...) };

			add_object(name, func, true);

			return (*this);
		}

		MODULE def_submodule(cstring name, cstring doc = "")
		{
			return nullptr;
		}

		void reload()
		{
		}

		template <class Name = cstring, class O = OBJECT
		> void add_object(Name && name, O && value, bool overwrite = false)
		{
			auto i{ object_or_cast(FWD(name)) };
			if (m_data.dict->contains(i) && !overwrite) { return; }
			m_data.dict->insert(std::move(i), object_or_cast(FWD(value)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		auto & d{ ***(get_interpstate()->modules) };
		auto i{ object_or_cast(name) };
		if (d.find(i) != d.end()) { return nullptr; }
		return d.try_emplace(std::move(i), MODULE::create(name)).first->second;
	}

	inline MODULE import_module(cstring name)
	{
		auto & d{ ***(get_interpstate()->modules) };
		auto i{ object_or_cast(name) };
		if (auto it{ d.find(i) }; it != d.end()) { return it->second; }
		return nullptr;
	}

	inline DICT globals()
	{
		if (auto frame{ get_framestate() }; frame) { return CHECK(frame->globals); }
		return import_module("__main__").attr("__dict__");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// generic
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreGeneric : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreGeneric;

		NODISCARD static auto type_static() { return CoreType_Type; }

	public:
		virtual ~CoreGeneric() override = default;
		explicit CoreGeneric(TYPE const & t) : base_type{ t } {}
		CoreGeneric() : base_type{ type_static() } {}
		CoreGeneric(self_type const &) = default;
		CoreGeneric(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// class
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Options
	> class CoreClass : public CoreGeneric
	{
	public:
		using base_type = CoreGeneric;
		using self_type = CoreClass<T>;
		using base_type::type_static;

	public:
		virtual ~CoreClass() override = default;

		template <class ... Extra
		> CoreClass(OBJECT scope, cstring name, Extra && ... extra) : base_type{}
		{
		}

		template <class Method, class ... Extra
		> self_type & def(cstring name, Method && method, Extra && ... extra)
		{
			return (*this);
		}

		template <class ... Args, class ... Extra
		> self_type & def(detail::initimpl::Constructor<Args...> && init, Extra && ... extra)
		{
			FWD(init).execute(*this, FWD(extra)...);
			return (*this);
		}

		template <class ... Args, class ... Extra
		> self_type & def(detail::initimpl::Factory<Args...> && init, Extra && ... extra)
		{
			FWD(init).execute(*this, FWD(extra)...);
			return (*this);
		}

		template <class Method, class ... Extra
		> self_type & def_static(cstring name, Method && method, Extra && ... extra)
		{
			return (*this);
		}

		template <class C, class D, class ... Extra
		> self_type & def_readwrite(cstring name, D C:: * pm, Extra && ... extra)
		{
			return (*this);
		}

		template <class C, class D, class ... Extra
		> self_type & def_readonly(cstring name, D const C:: * pm, Extra && ... extra)
		{
			return (*this);
		}

		template <class D, class ... Extra
		> self_type & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			return (*this);
		}

		template <class D, class ... Extra
		> self_type & def_readonly_static(cstring name, D const * pm, Extra && ... extra)
		{
			return (*this);
		}

		template <class Getter, class ... Extra
		> self_type & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return (*this);
		}

		template <class ... Extra
		> self_type & def_property_readonly(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return (*this);
		}

		template <class Getter, class ... Extra
		> self_type & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra)
		{
			return (*this);
		}

		template <class ... Extra
		> self_type & def_property_readonly_static(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return (*this);
		}

		template <class Getter, class Setter, class ... Extra
		> self_type & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra)
		{
			return (*this);
		}

		template <class Getter, class ... Extra
		> self_type & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return (*this);
		}

		template <class ... Extra
		> self_type & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return (*this);
		}

		template <class Getter, class ... Extra
		> self_type & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return (*this);
		}

		template <class ... Extra
		> self_type & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> NODISCARD auto init() -> detail::initimpl::Constructor<Args...> { return {}; }

	template <class Fn, class Rv = detail::initimpl::Factory<Fn>
	> NODISCARD auto init(Fn && fn) -> Rv { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BIND_HPP_
