#ifndef _ISM_CAST_HPP_
#define _ISM_CAST_HPP_

#include <core/api/internals.hpp>

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct type_caster_generic
	{
		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			return load_impl<type_caster_generic>(src, convert);
		}

		template <class ThisT> bool load_impl(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			return false;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	template <class T, class SFINAE = void> struct type_caster : type_caster_base<T> {};

	template <class T> ALIAS(make_caster) type_caster<intrinsic_t<T>>;

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
			if (!src) { return Core_None; } \
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
		template <class T> using cast_op_type = typename cast_op_type<T>; \

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct type_caster<std::reference_wrapper<T>>
	{
		using caster_t = make_caster<T>;

		caster_t subcaster;

		NODISCARD bool load(OBJECT const & src, bool convert) { return subcaster.load(src, convert); }

		static OBJECT cast(std::reference_wrapper<T> const & src) { return caster_t::cast(src.get()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct type_caster<T, std::enable_if_t<std::is_arithmetic_v<T> && !is_char_v<T>>>
	{
		using _type_0 = std::conditional_t<sizeof(T) <= sizeof(int32_t), int32_t, int64_t>;
		using _type_1 = std::conditional_t<std::is_signed_v<T>, _type_0, std::make_unsigned_t<_type_0>>;
		using convt_t = std::conditional_t<std::is_floating_point_v<T>, double_t, _type_1>;

		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			if (isinstance<INT>(src)) { value = (convt_t)(***INT(src)); return true; }
			if (isinstance<FLT>(src)) { value = (convt_t)(***FLT(src)); return true; }
			return false;
		}

		NODISCARD static OBJECT cast(T src, ReturnPolicy, OBJECT)
		{
			if constexpr (std::is_floating_point_v<T>) { return FLT::create(src); }
			else { return INT::create(src); }
		}

		DECL_TYPE_CASTER(T, std::is_integral_v<T> ? "int" : "float");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct void_caster
	{
		NODISCARD bool load(OBJECT const & src, bool) { return (src.is_valid() && src.is_none()); }

		NODISCARD static OBJECT cast(T, ReturnPolicy, OBJECT) { return Core_None; }

		DECL_TYPE_CASTER(T, "none");
	};

	template <> struct type_caster<void_type> : void_caster<void_type> {};

	template <> struct type_caster<void> : type_caster<void_type>
	{
		using type_caster<void_type>::cast;

		NODISCARD static OBJECT cast(void const * src, ReturnPolicy, OBJECT)
		{
			return src ? CAPSULE::create(src) : Core_None;
		}
	};

	template <> struct type_caster<nullptr_t>
		: void_caster<nullptr_t> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct type_caster<bool>
	{
		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			if (src == Core_True) { value = true; return true; }
			if (src == Core_False) { value = false; return true; }
			return false;
		}

		NODISCARD static OBJECT cast(bool src, ReturnPolicy, OBJECT) { return Core_Boolean(src); }

		DECL_TYPE_CASTER(bool, "bool");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct string_caster
	{
		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			value = ***STR(src);
			return true;
		}

		NODISCARD static OBJECT cast(T const & src, ReturnPolicy, OBJECT)
		{
			return STR::create(src);
		}

		DECL_TYPE_CASTER(T, "string");
	};

	template <class Ch, class Tr, class Al> struct type_caster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<is_char_v<Ch>>>
		: string_caster<std::basic_string<Ch, Tr, Al>> {};

	template <class T> struct type_caster<T, std::enable_if_t<is_char_v<T>>> : type_caster<std::basic_string<T>>
	{
		type_caster<BasicString<T>> str_caster;

		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			return str_caster.load(src, convert);
		}

		NODISCARD static auto cast(T const * src, ReturnPolicy, OBJECT)
		{
			return STR::create(src);
		}

		NODISCARD static auto cast(T src, ReturnPolicy, OBJECT)
		{
			return INT::create(src);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct core_caster
	{
		template <class U = T, std::enable_if_t<std::is_same_v<U, OBJECT>, int> = 0
		> NODISCARD bool load(OBJECT const & src, bool) { value = src; return static_cast<bool>(value); }

		template <class U = T, std::enable_if_t<is_handle_v<U>, int> = 0
		> NODISCARD bool load(OBJECT const & src, bool)
		{
			if (!isinstance<T>(src)) { return false; }
			value = src;
			return true;
		}

		NODISCARD static OBJECT cast(OBJECT const & src, ReturnPolicy, OBJECT)
		{
			return src;
		}

		DECL_TYPE_CASTER(T, "object");
	};

	template <class T> struct type_caster<T, std::enable_if_t<is_object_api_v<T>>>
		: core_caster<T> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE
	> type_caster<T, SFINAE> & load_type(type_caster<T, SFINAE> & conv, OBJECT const & o)
	{
		VERIFY(conv.load(o, true));
		return conv;
	}

	template <typename T> make_caster<T> load_type(OBJECT const & o)
	{
		make_caster<T> conv;
		load_type(conv, o);
		return conv;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// obj -> c++
	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD T cast(OBJECT const & o)
	{
		return detail::cast_op<T>(detail::load_type<T>(o));
	}

	// obj -> obj
	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD T cast(OBJECT const & o) { return T{ o }; }

	// c++ -> obj
	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD OBJECT cast(T && o, ReturnPolicy policy = ReturnPolicy::AutomaticReference, OBJECT parent = {})
	{
		using U = std::remove_reference_t<T>;

		if (policy == ReturnPolicy::Automatic) {
			policy = (std::is_pointer_v<U>
				? ReturnPolicy::TakeOwnership
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy::Copy
					: ReturnPolicy::Move));
		} else if (policy == ReturnPolicy::AutomaticReference) {
			policy = (std::is_pointer_v<U>
				? ReturnPolicy::Reference
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy::Copy
					: ReturnPolicy::Move));
		}

		return detail::make_caster<T>::cast(FWD(o), policy, parent);
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

	template <class T, std::enable_if_t<!is_object_api_v<T>, int>
	> NODISCARD OBJECT object_or_cast(T && o) { return ism::cast(FWD(o)); }

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

		std::tuple<detail::make_caster<Args>...> argcasters;

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
			return f(cast_op<Args>(std::move(std::get<I>(argcasters)))...);
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
			v.push_back(object_or_cast(FWD(e)));
		}, FWD(args)...);
		return v;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> template <class ...Args
	> inline OBJECT ObjectAPI<Derived>::operator()(Args && ... args_in)
	{
		auto args{ LIST::create(collect_arguments(FWD(args_in)...)) };

		return Core_None;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAST_HPP_
