#ifndef _ISM_CAST_DETAIL_HPP_
#define _ISM_CAST_DETAIL_HPP_

#include <core/api/internals.hpp>

// info
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline TYPE typeof_generic(std::type_info const & t)
	{
		return nullptr;
	}

	NODISCARD inline bool isinstance_generic(OBJECT const & o, std::type_info const & t)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// base casters
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type_caster_generic
	struct type_caster_generic
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> using cast_op_type =
		std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
		typename std::add_pointer_t<mpl::intrinsic_t<T>>,
		typename std::add_lvalue_reference_t<mpl::intrinsic_t<T>>>;

	template <class T
	> using movable_cast_op_type =
		std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
		typename std::add_pointer_t<mpl::intrinsic_t<T>>,
		std::conditional_t<std::is_rvalue_reference_v<T>,
		typename std::add_rvalue_reference_t<mpl::intrinsic_t<T>>,
		typename std::add_lvalue_reference_t<mpl::intrinsic_t<T>>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type_caster_base
	template <class T> struct type_caster_base : type_caster_generic
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct type_caster : type_caster_base<T> {};

	template <class T> ALIAS(make_caster) typename type_caster<mpl::intrinsic_t<T>>;

	template <class T> auto cast_op(make_caster<T> & caster) -> typename make_caster<T>::template cast_op_type<T>
	{
		return caster.operator typename make_caster<T>::template cast_op_type<T>();
	}

	template <class T> auto cast_op(make_caster<T> && caster) -> typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>
	{
		return std::move(caster).operator typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}

// type casters
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_TYPE_CASTER(m_type, m_name) \
	protected: \
		m_type value; \
	public: \
		static constexpr auto name{ m_name }; \
		template <class T, std::enable_if_t<std::is_same_v<m_type, std::remove_cv_t<T>>, int> = 0 \
		> static OBJECT cast(T * src, ReturnPolicy policy, OBJECT parent) \
		{ \
			if (!src) { return nullptr; } \
			if (policy == ReturnPolicy_TakeOwnership) \
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
		template <class T> using cast_op_type = api::movable_cast_op_type<T>; \

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct type_caster<std::reference_wrapper<T>>
	{
		using caster_t = make_caster<T>;

		caster_t subcaster;

		NODISCARD bool load(OBJECT const & src, bool convert) { return subcaster.load(src, convert); }

		static OBJECT cast(std::reference_wrapper<T> const & src) { return caster_t::cast(src.get()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct type_caster<T, std::enable_if_t<mpl::is_numeric_v<T>>>
	{
		using _itype = IntObject::storage_type;
		using _ftype = FloatObject::storage_type;
		using _type0 = std::conditional_t<sizeof(T) <= sizeof(int32_t), int32_t, int64_t>;
		using _type1 = std::conditional_t<std::is_signed_v<T>, _type0, std::make_unsigned_t<_type0>>;
		using _convt = std::conditional_t<std::is_floating_point_v<T>, double_t, _type1>;

		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }

			if (isinstance<FLT>(src)) { value = (_convt)(_ftype)FLT(src); return true; }

			if (isinstance<INT>(src)) { value = (_convt)(_itype)INT(src); return true; }

			return false;
		}

		NODISCARD static OBJECT cast(T src, ReturnPolicy, OBJECT)
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				return FLT(static_cast<_ftype>(src));
			}
			else
			{
				return INT(static_cast<_itype>(src));
			}
		}

		ISM_TYPE_CASTER(T, std::is_integral_v<T> ? "int" : "float");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct void_caster
	{
		NODISCARD bool load(OBJECT const & src, bool) { return (src.is_valid() && src.is_null()); }

		NODISCARD static OBJECT cast(T, ReturnPolicy, OBJECT) { return nullptr; }

		ISM_TYPE_CASTER(T, "none");
	};

	template <> struct type_caster<void_type> : void_caster<void_type> {};

	template <> struct type_caster<nullptr_t> : void_caster<nullptr_t> {};

	template <> struct type_caster<void> : type_caster<void_type>
	{
		NODISCARD bool load(OBJECT const & src, bool)
		{
			if (src.is_null())
			{
				value = nullptr;
				return true;
			}
			else if (isinstance<CAPSULE>(src))
			{
				value = CAPSULE(src).get_pointer();
				return true;
			}
			else
			{
				return false;
			}
		}

		template <class U> NODISCARD static OBJECT cast(U src, ReturnPolicy, OBJECT)
		{
			return src ? CAPSULE({ static_cast<void const *>(src) }) : nullptr;
		}
		
		static constexpr auto name{ "capsule" };
		
		template <typename T> using cast_op_type = void *&;
		
		NODISCARD operator void *& () { return value; }

	private:
		void * value{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct type_caster<bool>
	{
		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			else if (src == Core_True) { value = true; return true; }
			else if (src == Core_False) { value = false; return true; }
			else return false;
		}

		NODISCARD static OBJECT cast(bool src, ReturnPolicy, OBJECT) { return Core_Bool(src); }

		ISM_TYPE_CASTER(bool, "bool");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct string_caster
	{
		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			value = (String)STR(src);
			return true;
		}

		NODISCARD static OBJECT cast(T const & src, ReturnPolicy, OBJECT)
		{
			return STR(src);
		}

		ISM_TYPE_CASTER(T, "string");
	};

	template <class Ch, class Tr, class Al
	> struct type_caster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<mpl::is_char_v<Ch>>>
		: string_caster<std::basic_string<Ch, Tr, Al>> {};

	template <class T> struct type_caster<T, std::enable_if_t<mpl::is_char_v<T>>> : type_caster<std::basic_string<T>>
	{
		type_caster<BasicString<T>> str_caster;

		NODISCARD bool load(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			return str_caster.load(src, convert);
		}

		NODISCARD static OBJECT cast(T const * src, ReturnPolicy, OBJECT)
		{
			return STR(src);
		}

		NODISCARD static OBJECT cast(T const src, ReturnPolicy, OBJECT)
		{
			return INT(src);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct holder_helper
	{
		static auto get(T const & p) -> decltype(p.get()) { return p.get(); }
	};

	template <class T, class Holder
	> struct copyable_holder_caster : type_caster_base<T>
	{
		// for types like shared_ptr, unique_ptr, etc...
	};

	template <class T> struct type_caster<std::shared_ptr<T>> : copyable_holder_caster<T, std::shared_ptr<T>>
	{
		// specialize for the common shared_ptr, so users don't need to
	};

	template <class T, class Holder
	> struct move_only_holder_caster
	{
		static_assert(
			std::is_base_of_v<type_caster_base<T>, type_caster<T>>,
			"holder classes are only supported for custom types");

		static OBJECT cast(Holder && src, ReturnPolicy, OBJECT)
		{
			auto ptr{ holder_helper<Holder>::get(src) };
			return type_caster_base<T>::cast_holder(ptr, std::addressof(src));
		}
	};

	template <class T, class Deleter
	> struct type_caster<std::unique_ptr<T, Deleter>>
		: move_only_holder_caster<T, std::unique_ptr<T, Deleter>> { };

	template <class T, class Holder
	> using type_caster_holder = std::conditional_t<
		mpl::is_copy_constructible_v<Holder>,
		copyable_holder_caster<T, Holder>,
		move_only_holder_caster<T, Holder>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, bool Value = false
	> struct always_construct_holder { static constexpr bool value = Value; };

	template <class T, class Holder
	> struct is_holder_type : std::is_base_of<type_caster_holder<T, Holder>, type_caster<Holder>> {};

	template <class T, class Deleter
	> struct is_holder_type<T, std::unique_ptr<T, Deleter>> : std::true_type {};

	// move_is_plain_type
	template <class T> using move_is_plain_type = mpl::satisfies_none_of<T,
		std::is_void,
		std::is_pointer,
		std::is_reference,
		std::is_const>;
	template <class T> constexpr bool move_is_plain_type_v{ move_is_plain_type<T>::value };

	// move_always
	template <class T, class SFINAE = void> struct move_always : std::false_type {};
	template <class T> struct move_always<T, std::enable_if_t<mpl::all_of_v<
		move_is_plain_type<T>,
		std::negation<mpl::is_copy_constructible<T>>,
		std::is_move_constructible<T>,
		std::is_same<decltype(std::declval<make_caster<T>>().operator T & ()), T &>
		>>> : std::true_type {};
	template <class T> constexpr bool move_always_v{ move_always<T>::value };

	// move_if_unreferenced
	template <class T, class SFINAE = void> struct move_if_unreferenced : std::false_type {};
	template <class T> struct move_if_unreferenced<T, std::enable_if_t<mpl::all_of_v<
		move_is_plain_type<T>,
		std::negation<move_always<T>>,
		std::is_move_constructible<T>,
		std::is_same<decltype(std::declval<make_caster<T>>().operator T & ()), T &>
		>>> : std::true_type {};
	template <class T> constexpr bool move_if_unreferenced_v{ move_if_unreferenced<T>::value };

	// move_never
	template <class T> using move_never = mpl::none_of<move_always<T>, move_if_unreferenced<T>>;
	template <class T> constexpr bool move_never_v{ move_never<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct object_caster
	{
		template <class U = T
		> NODISCARD bool load(Handle<U> const & src, bool)
		{
			if (!isinstance<T>(src)) { return false; }
			value = src;
			return true;
		}

		NODISCARD static OBJECT cast(OBJECT const & src, ReturnPolicy, OBJECT)
		{
			return src;
		}

		ISM_TYPE_CASTER(T, "object");
	};

	template <class T> struct type_caster<T, std::enable_if_t<is_object_api_v<T>>>
		: object_caster<T> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// cast
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T, class SFINAE = void
	> struct return_policy_override { static ReturnPolicy policy(ReturnPolicy p) { return p; } };

	template <class T> struct return_policy_override<T, std::enable_if_t<std::is_base_of_v<api::type_caster_generic, api::make_caster<T>>, void>>
	{
		static ReturnPolicy policy(ReturnPolicy p) { return !std::is_lvalue_reference_v<T> && !std::is_pointer_v<T> ? ReturnPolicy_Move : p; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE
	> auto load_type(api::type_caster<T, SFINAE> & convt, OBJECT const & o) -> api::type_caster<T, SFINAE> &
	{
		if (!convt.load(o, true)) {
			VERIFY(!"TYPE CONVERSION FAILED");
		}
		return convt;
	}

	template <class T
	> auto load_type(OBJECT const & o) -> make_caster<T>
	{
		api::make_caster<T> convt;
		load_type(convt, o);
		return convt;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// api -> c++
	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD T cast(OBJECT const & o) { return cast_op<T>(load_type<T>(o)); }

	// api -> api
	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD T cast(OBJECT const & o) { return T{ o }; }

	// c++ -> api
	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD OBJECT cast(T && o, ReturnPolicy policy = ReturnPolicy_AutomaticReference, OBJECT parent = {})
	{
		if (policy == ReturnPolicy_Automatic) {
			policy = (std::is_pointer_v<std::remove_reference_t<T>>
				? ReturnPolicy_TakeOwnership
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy_Copy
					: ReturnPolicy_Move));
		}
		else if (policy == ReturnPolicy_AutomaticReference) {
			policy = (std::is_pointer_v<std::remove_reference_t<T>>
				? ReturnPolicy_Reference
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy_Copy
					: ReturnPolicy_Move));
		}
		return OBJECT{ make_caster<T>::cast(FWD(o), policy, parent) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> auto move(OBJECT && o) -> std::enable_if_t<!move_never_v<T>, T>
	{
		if (o && o->ref_count() > 1) {
			VERIFY(!"Unable to cast Core instance to C++ rvalue: instance has multiple references (compile in debug mode for details)");
		}
		T ret{ std::move(api::load_type<T>(o).operator T & ()) };
		return ret;
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<move_always_v<T>, T>
	{
		return api::move<T>(std::move(o));
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<move_if_unreferenced_v<T>, T>
	{
		if (o && o->ref_count() > 1)
		{
			return api::cast<T>(o);
		}
		else
		{
			return api::move<T>(std::move(o));
		}
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<move_never_v<T>, T>
	{
		return api::cast<T>(o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<!is_object_api_v<T>, int>
	> NODISCARD OBJECT object_or_cast(T && o) { return api::cast(FWD(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O> template <class T> inline T BaseHandle<O>::cast() const &
	{
		if constexpr (!std::is_void_v<T>) { return api::cast<T>(*this); }
	}

	template <class O> template <class T> inline T BaseHandle<O>::cast() &&
	{
		if constexpr (!std::is_void_v<T>) { return api::cast<T>(std::move(*this)); }
	}

	template <class T> inline T BaseObject::cast() const &
	{
		if constexpr (!std::is_void_v<T>) { return api::cast<T>(*this); }
	}

	template <class T> inline T BaseObject::cast() &&
	{
		if constexpr (!std::is_void_v<T>) { return api::cast<T>(std::move(*this)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAST_DETAIL_HPP_
