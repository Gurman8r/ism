#ifndef _ISM_CAST_HPP_
#define _ISM_CAST_HPP_

#include <core/api/runtime.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct type_caster_generic
	{
	};

	template <class T> struct type_caster_base : type_caster_generic
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> using cast_op_type =
		std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
		typename std::add_pointer_t<intrinsic_t<T>>,
		typename std::add_lvalue_reference_t<intrinsic_t<T>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl { template <class T, class SFINAE = void> struct type_caster : type_caster_base<T> {}; }

	template <class T> ALIAS(make_caster) impl::type_caster<intrinsic_t<T>>;

	template <class T
	> auto cast_op(make_caster<T> & caster) -> typename make_caster<T>::template cast_op_type<T>
	{
		return caster.operator typename make_caster<T>::template cast_op_type<T>();
	}

	template <class T
	> auto cast_op(make_caster<T> && caster) -> typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference<T>::type>
	{
		return std::move(caster).operator
			typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference<T>::type>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DECL_TYPE_CASTER(m_type, m_name) \
	protected: \
		m_type value; \
	public: \
		static constexpr auto name{ m_name }; \
		template <class T, std::enable_if_t<std::is_same_v<m_type, std::remove_cv_t<T>>, int> = 0 \
		> static OBJECT cast(T * src, ReturnPolicy policy, OBJECT parent) \
		{ \
			if (!src) { return Core_Null; } \
			if (policy == ReturnPolicy::TakeOwnership) \
			{ \
				auto h{ cast(std::move(*src), policy, parent) }; \
				delete src; \
				return h; \
			} \
			else \
			{ \
				return cast(*src, policy, parent); \
			} \
		} \
		operator m_type * () { return &value; } \
		operator m_type & () { return value; } \
		operator m_type && () { return std::move(value); } \
		template <class T> using cast_op_type = typename ism::cast_op_type<T>; \

	namespace impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<std::reference_wrapper<T>>
		{
			using caster_t = make_caster<T>;

			caster_t subcaster;

			bool load(OBJECT src, bool convert) { return subcaster.load(src, convert); }

			static OBJECT cast(std::reference_wrapper<T> const & src)
			{
				return caster_t::cast(src.get());
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<T, std::enable_if_t<std::is_arithmetic_v<T> && !is_char_v<T>>>
		{
			static constexpr bool integer{ std::is_integral_v<T> };

			using handle_t = std::conditional_t<integer, INT, FLT>;

			using convt_t = std::conditional_t<sizeof(void *) == sizeof(int32_t),
				std::conditional_t<integer, int32_t, float32_t>,
				std::conditional_t<integer, int64_t, float64_t>>;

			bool load(OBJECT src, bool convert)
			{
				return false;
			}

			NODISCARD static OBJECT cast(T src, ReturnPolicy, OBJECT)
			{
				return ism::make_object<handle_t>(src);
			}

			DECL_TYPE_CASTER(T, integer ? "int" : "float");
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct void_caster
		{
			NODISCARD static OBJECT cast(T, ReturnPolicy, OBJECT)
			{
				return nullptr;
			}

			DECL_TYPE_CASTER(T, "none");
		};

		template <> struct type_caster<void_type> : void_caster<void_type> {};

		template <
		> struct type_caster<void> : type_caster<void_type>
		{
			using type_caster<void_type>::cast;

			NODISCARD static OBJECT cast(void const * src, ReturnPolicy, OBJECT)
			{
				return !src ? nullptr : CAPSULE::create(src);
			}
		};

		template <
		> struct type_caster<nullptr_t>
			: void_caster<nullptr_t> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
		> struct type_caster<bool>
		{
			bool load(OBJECT src, bool convert)
			{
				if (!src) { return false; }
				else if (src == Core_True) { value = true; return true; }
				else if (src == Core_False) { value = false; return true; }
				else return false;
			}

			NODISCARD static OBJECT cast(bool src, ReturnPolicy, OBJECT) { return Core_Boolean(src); }

			DECL_TYPE_CASTER(bool, "bool");
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S
		> struct string_caster
		{
			using string_type = typename S;

			bool load(OBJECT src, bool convert)
			{
				return false;
			}

			NODISCARD static OBJECT cast(string_type const & src, ReturnPolicy, OBJECT)
			{
				return STR::create(src);
			}

			DECL_TYPE_CASTER(string_type, "string");
		};

		template <class Ch, class Tr, class Al
		> struct type_caster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<is_char_v<Ch>>>
			: string_caster<std::basic_string<Ch, Tr, Al>> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<T, std::enable_if_t<is_char_v<T>>> : type_caster<std::basic_string<T>>
		{
			using type_caster<std::basic_string<T>>::cast;

			NODISCARD static auto cast(T const src, ReturnPolicy, OBJECT)
			{
				return INT::create(src);
			}

			NODISCARD static auto cast(T const * src, ReturnPolicy, OBJECT)
			{
				return STR::create(src);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct core_caster
		{
			template <class U = T, std::enable_if_t<std::is_same_v<U, OBJECT>, int> = 0
			> bool load(OBJECT src, bool) { value = src; return static_cast<bool>(value); }

			template <class U = T, std::enable_if_t<is_handle_v<U>, int> = 0
			> bool load(OBJECT src, bool)
			{
				if (!ism::isinstance<T>(src)) { return false; }
				value = src;
				return true;
			}

			NODISCARD static OBJECT cast(OBJECT const & src, ReturnPolicy, OBJECT)
			{
				return src;
			}

			DECL_TYPE_CASTER(T, "object");
		};

		template <class T
		> struct type_caster<T, std::enable_if_t<is_api_v<T>>>
			: core_caster<T> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// core -> core
	template <class T, class O, std::enable_if_t<is_api_v<T> && is_api_v<O>, int> = 0
	> NODISCARD decltype(auto) cast(O && o) { return T{ FWD(o) }; }

	// core -> c++
	template <class T, class O, std::enable_if_t<!is_api_v<T> && is_api_v<O>, int> = 0
	> NODISCARD decltype(auto) cast(O && o)
	{
		if constexpr (std::is_same_v<O, make_handle<T>>)
		{
			return static_cast<T>(***o);
		}
		else
		{
			return ism::cast<T>(ism::cast<make_handle<T>>(FWD(o)));
		}
	}

	// c++ -> core
	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD OBJECT cast(T && o, ReturnPolicy policy = ReturnPolicy::AutomaticReference, OBJECT parent = {})
	{
		using U = std::remove_reference_t<T>;

		if (policy == ReturnPolicy::Automatic)
		{
			policy = (std::is_pointer_v<U>
				? ReturnPolicy::TakeOwnership
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy::Copy
					: ReturnPolicy::Move));
		}
		else if (policy == ReturnPolicy::AutomaticReference)
		{
			policy = (std::is_pointer_v<U>
				? ReturnPolicy::Reference
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy::Copy
					: ReturnPolicy::Move));
		}

		return ism::make_caster<T>::cast(FWD(o), policy, parent);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O> template <class T> inline T Handle<O>::cast() const &
	{
		if constexpr (!std::is_same_v<T, void>) { return ism::cast<T>(*this); }
	}

	template <class O> template <class T> inline T Handle<O>::cast() &&
	{
		if constexpr (!std::is_same_v<T, void>) { return ism::cast<T>(std::move(*this)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<!is_api_v<T>, int>
	> NODISCARD OBJECT object_or_cast(T && o)
	{
		return ism::cast(FWD(o));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FunctionRecord;

	struct FunctionCall
	{
		FunctionCall(FunctionRecord const & f, OBJECT parent);

		FunctionRecord const & func;

		Vector<OBJECT> args{};

		Vector<bool> args_convert{};

		OBJECT parent{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct ArgumentLoader
	{
	public:
		static constexpr size_t nargs{ sizeof...(Args) };

		using indices = std::make_index_sequence<nargs>;

		std::tuple<ism::make_caster<Args>...> argcasters;

		bool load_args(FunctionCall & call)
		{
			return impl_load_sequence(call, indices{});
		}

		template <class R, class F
		> auto call(F && f) && -> std::conditional_t<std::is_void_v<R>, void_type, R>
		{
			if constexpr (std::is_void_v<R>)
			{
				std::move(*this).template call_impl<R>(FWD(f), indices{});
				return void_type{};
			}
			else
			{
				return std::move(*this).template call_impl<R>(FWD(f), indices{});
			}
		}

	private:
		static bool impl_load_sequence(FunctionCall &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_sequence(FunctionCall & call, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(call.args[I], call.args_convert[I]));
		}

		template <class R, class F, size_t ... I
		> R call_impl(F && f, std::index_sequence<I...>) &&
		{
			return f(ism::cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Arg
	{
		constexpr explicit Arg(cstring name = nullptr) : name{ name }, flag_convert{ false }, flag_none{ true } {}

		Arg & noconvert(bool value = true) { flag_convert = value; return (*this); }
		Arg & none(bool value = true) { flag_none = value; return (*this); }

		cstring name;

		bool flag_convert, flag_none;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> auto collect_arguments(Args && ... args) noexcept -> Vector<OBJECT>
	{
		Vector<OBJECT> v{};
		v.reserve(sizeof...(Args));
		mpl::for_args([&v](auto && e) noexcept {
			v.push_back(ism::object_or_cast(FWD(e)));
		}, FWD(args)...);
		return v;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<class Derived
	> template <class ...Args
	> inline OBJECT ObjectAPI<Derived>::operator()(Args && ... args_in)
	{
		auto args{ LIST::create(ism::collect_arguments(FWD(args_in)...)) };

		return Core_Null;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAST_HPP_
