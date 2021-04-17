#ifndef _ISM_MODSUPPORT_HPP_
#define _ISM_MODSUPPORT_HPP_

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

		FunctionRecord m_rec{};

	public:
		CoreCppFunction() : base_type{} {}
		CoreCppFunction(nullptr_t) : base_type{} {}
		CoreCppFunction(self_type const &) = default;
		CoreCppFunction(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		virtual ~CoreCppFunction() override
		{
			if (m_rec.free_data) { m_rec.free_data(&m_rec); }
		}

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
			this->initialize([f](C * c, Args ... args) -> R { return (c->*f)(args...); }, (R(*)(C *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) &, Extra && ... extra)
		{
			this->initialize([f](C * c, Args ... args) -> R { return (c->*f)(args...); }, (R(*)(C *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) const, Extra && ... extra)
		{
			this->initialize([f](C const * c, Args ... args) -> R { return (c->*f)(args...); }, (R(*)(C const *, Args...))nullptr, FWD(extra)...);
		}

		template <class R, class C, class ... Args, class ... Extra
		> CoreCppFunction(R(C:: * f)(Args...) const &, Extra && ... extra)
		{
			this->initialize([f](C const * c, Args ... args) -> R { return (c->*f)(args...); }, (R(*)(C const *, Args...))nullptr, FWD(extra)...);
		}

		NODISCARD OBJECT name() const { return attr("__name__"); }

	protected:
		template <class Fn, class Return, class ... Args, class ... Extra
		> void initialize(Fn && f, Return(*)(Args...), Extra && ... extra)
		{
			struct Capture { std::remove_reference_t<Fn> f; };

			if (sizeof(Capture) <= sizeof(m_rec.data))
			{
				::new((Capture *)&m_rec.data) Capture{ FWD(f) };

				if constexpr (!std::is_trivially_destructible_v<Fn>)
				{
					m_rec.free_data = [](FunctionRecord * r)
					{
						((Capture *)&r->data)->~Capture();
					};
				}
			}
			else
			{
				m_rec.data[0] = memnew(Capture{ FWD(f) });

				m_rec.free_data = [](FunctionRecord * r)
				{
					ism::memdelete((Capture *)r->data[0]);
				};
			}

			using cast_in = argument_loader<Args...>;
			using cast_out = detail::make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

			m_rec.impl = [](FunctionCall & call) -> OBJECT
			{
				cast_in args{};
				if (!args.load_args(call)) { return TRY_NEXT_OVERLOAD; }

				process_attributes<Extra...>::precall(call);

				auto data{ (sizeof(Capture) <= sizeof(call.func.data) ? &call.func.data : call.func.data[0]) };

				auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnPolicy policy{ detail::return_policy_override<Return>::policy(call.func.policy) };

				using Guard = extract_guard_t<Extra...>;

				OBJECT result{ cast_out::cast(std::move(args).call<Return, Guard>(capture->f), policy, call.parent) };

				process_attributes<Extra...>::postcall(call, result);

				return result;
			};

			if constexpr (std::is_convertible_v<Fn, Return(*)(Args...)> && sizeof(Capture) == sizeof(void *))
			{
				m_rec.is_stateless = true;
				m_rec.data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
			}

			m_rec.nargs = sizeof...(Args);
			m_rec.args.reserve(m_rec.nargs);
			for (size_t i = 0; i < m_rec.nargs; ++i)
			{
				auto & arg{ m_rec.args.emplace_back(ArgumentRecord{}) };
				arg.name = "";
				arg.value = OBJECT{};
				arg.convert = false;
				arg.none = false;
			}

			m_vectorcall = [](CPP_FUNCTION self, OBJECT const * argv, size_t argc) -> OBJECT
			{
				OBJECT parent{ 0 < argc ? argv[0] : nullptr };

				FunctionRecord const & func{ self->m_rec };

				FunctionCall call{ func, parent };

				size_t args_to_copy{ MIN(func.nargs, argc) }, args_copied{};

				// copy passed arguments
				bool bad_arg{};
				for (; args_copied < args_to_copy; ++args_copied)
				{
					ArgumentRecord const * arg_rec{ args_copied < func.args.size() ? &func.args[args_copied] : nullptr };
					OBJECT arg{ argv[args_copied] };
					if (arg_rec && !arg_rec->none && arg.is_none())
					{
						bad_arg = true;
						break;
					}
					call.args.push_back(arg);
					call.args_convert.push_back(arg_rec ? arg_rec->convert : false);
				}
				VERIFY(!bad_arg);

				// fill in missing args
				if (args_copied < func.nargs)
				{
					for (; args_copied < args_to_copy; ++args_copied)
					{
						if (auto const & arg{ func.args[args_copied] }; arg.value)
						{
							call.args.push_back(arg.value);
							call.args_convert.push_back(arg.convert);
						}
						else
						{
							break;
						}
					}
					VERIFY(func.nargs <= args_copied);
				}

				OBJECT result{};

				result = func.impl(call);

				return result;
			};
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// module
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreModule_Type;
#define CoreModule_Type		(ism::TYPE(&ism::_CoreModule_Type))

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreModule;

		NODISCARD static auto type_static() { return CoreModule_Type; }

		DICT		m_dict{};
		STR			m_name{};
		STR			m_doc{};
		inquiry		m_clear{};
		freefunc	m_free{};

	public:
		CoreModule() : base_type{ type_static() } {}
		CoreModule(self_type const &) = default;
		CoreModule(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		template <class O
		> CoreModule(Handle<O> const & o) : self_type{}
		{
		}

		CoreModule(cstring name) : self_type{}
		{
			m_dict = DICT::create();
			m_name = STR::create(name);
		}

		template <class F, class ... Extra
		> self_type & def(cstring name, F && f, Extra && ... extra)
		{
			auto func{ CPP_FUNCTION::create(
				FWD(f),
				_Name{ name },
				_Scope{ handle() },
				_Sibling{ getattr(handle(), name, nullptr) },
				FWD(extra)...) };

			add_object(name, func, true);

			return (*this);
		}

		MODULE def_submodule(cstring name, cstring doc = "")
		{
			return nullptr;
		}

		template <class Name = cstring, class O = OBJECT
		> void add_object(Name && name, O && value, bool overwrite = false)
		{
			auto i{ object_or_cast(FWD(name)) };
			if (m_dict->contains(i) && !overwrite) { return; }
			m_dict->insert(std::move(i), object_or_cast(FWD(value)));
		}

		void reload()
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_interpreter()->modules };
		auto i{ object_or_cast(name) };
		if (d->contains(i)) { return nullptr; }
		return MODULE(d[i] = MODULE::create(name));
	}

	inline MODULE import_module(cstring name)
	{
		DICT d{ get_interpreter()->modules };
		auto i{ object_or_cast(name) };
		if (!d->contains(i)) { return nullptr; }
		return MODULE(d[i]);
	}

	inline DICT globals()
	{
		if (auto frame{ get_stack_frame() }; frame) { return CHECK(frame->globals); }
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

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline std::pair<decltype(RuntimeState::_registry::registered_types_core)::iterator, bool> all_type_info_get_cache(TYPE const & t)
	{
		auto res{ get_registry().registered_types_core.try_emplace(t) };
		if (res.second)
		{
			// set up a weak reference to automatically remove it if the type gets destroyed
		}
		return res;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODSUPPORT_HPP_
