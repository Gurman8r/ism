#ifndef _ISM_CAST_HPP_
#define _ISM_CAST_HPP_

#include <core/object/base_object.hpp>
#include <core/object/type_object.hpp>
#include <core/object/int_object.hpp>
#include <core/object/float_object.hpp>
#include <core/object/string_object.hpp>
#include <core/object/list_object.hpp>
#include <core/object/dict_object.hpp>
#include <core/object/capsule_object.hpp>
#include <core/object/function_object.hpp>
#include <core/object/method_object.hpp>
#include <core/object/property_object.hpp>

// misc
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// loader life support
	struct LoaderLifeSupport final
	{
		LoaderLifeSupport() noexcept
		{
			Internals::get_singleton()->loader_stack.push_back(nullptr);
		}

		~LoaderLifeSupport() noexcept
		{
			Vector<OBJ> & stack{ Internals::get_singleton()->loader_stack };
			ASSERT(!stack.empty());
			OBJ & ptr{ stack.back() };
			stack.pop_back();
			ptr = nullptr;
		}

		static void add(OBJ const & value) noexcept
		{
			Vector<OBJ> & stack{ Internals::get_singleton()->loader_stack };
			ASSERT(!stack.empty());
			LIST & list{ (LIST &)stack.back() };
			if (!list) { list = LIST::new_(); }
			list.append(value);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// base casters
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// generic caster
	struct TypeCasterGeneric
	{
		// TODO...
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

	// base caster
	template <class T> struct TypeCasterBase : TypeCasterGeneric
	{
		// TODO...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}

// type casters
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct TypeCaster : TypeCasterBase<T> {};

	template <class T> ALIAS(make_caster) TypeCaster<mpl::intrinsic_t<T>>;

	template <class T> auto cast_op(make_caster<T> & caster) -> typename make_caster<T>::template cast_op_type<T>
	{
		return caster.operator typename make_caster<T>::template cast_op_type<T>();
	}

	template <class T> auto cast_op(make_caster<T> && caster) -> typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>
	{
		return std::move(caster).operator typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TYPE_CASTER_COMMON(m_type, m_name)														\
protected:																						\
	m_type value{};																				\
																								\
public:																							\
	static constexpr auto name{ m_name };														\
																								\
	operator m_type * () { return &value; }														\
																								\
	operator m_type & () { return value; }														\
																								\
	operator m_type && () { return std::move(value); }											\
																								\
	template <class T_> ALIAS(cast_op_type) movable_cast_op_type<T_>;							\
																								\
	template <class T_, std::enable_if_t<std::is_same_v<m_type, std::remove_cv_t<T_>>, int> = 0	\
	> static OBJ cast(T_ * src, ReturnValuePolicy_ policy, OBJ const & parent)					\
	{																							\
		if (!src) { return nullptr; }															\
		else if (policy == ReturnValuePolicy_TakeOwnership)										\
		{																						\
			OBJ h{ cast(std::move(*src), policy, parent) };										\
			memdelete(src);																		\
			return h;																			\
		}																						\
		else																					\
		{																						\
			return cast(*src, policy, parent);													\
		}																						\
	}																							\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct TypeCaster<std::reference_wrapper<T>>
	{
		using caster_t = make_caster<T>;

		caster_t subcaster;

		bool load(OBJ const & src, bool convert) { return subcaster.load(src, convert); }

		static OBJ cast(std::reference_wrapper<T> const & src) { return caster_t::cast(src.get()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct TypeCaster<T, std::enable_if_t<mpl::is_number_v<T>>>
	{
		using _itype = IntObject::storage_type;
		using _ftype = FloatObject::storage_type;
		using _type0 = std::conditional_t<sizeof(T) <= sizeof(int32_t), int32_t, int64_t>;
		using _type1 = std::conditional_t<std::is_signed_v<T>, _type0, std::make_unsigned_t<_type0>>;
		using _convt = std::conditional_t<std::is_floating_point_v<T>, double_t, _type1>;

		bool load(OBJ const & src, bool convert)
		{
			if (!src) { return false; }
			
			else if (FLT::check_(src)) { return (value = (_convt)(_ftype)FLT(src)), true; }
			
			else if (INT::check_(src)) { return (value = (_convt)(_itype)INT(src)), true; }
			
			else { return false; }
		}

		static OBJ cast(T src, ReturnValuePolicy_, OBJ const &)
		{
			if constexpr (std::is_floating_point_v<T>) { return FLT(static_cast<_ftype>(src)); }

			else { return INT(static_cast<_itype>(src)); }
		}

		TYPE_CASTER_COMMON(T, std::is_floating_point_v<T> ? "float" : "int");
	};

	template <> struct TypeCaster<Duration>
	{
		bool load(OBJ const & src, bool convert)
		{
			if (!src) { return false; }

			if (FLT::check_(src)) { return (value = Duration{ (float_t)(***(FLT &)src) }), true; }

			else if (INT::check_(src)) { return (value = Duration{ (float_t)(***(INT &)src) }), true; }

			else { return false; }
		}

		static OBJ cast(Duration const & src, ReturnValuePolicy_, OBJ const &)
		{
			return FLT(src.count());
		}

		TYPE_CASTER_COMMON(Duration, "duration");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct VoidCaster
	{
		bool load(OBJ const & src, bool) { return src.is_valid(); }

		static OBJ cast(T, ReturnValuePolicy_, OBJ const &) { return OBJ{}; }

		TYPE_CASTER_COMMON(T, "none");
	};

	template <> struct TypeCaster<void_type> : VoidCaster<void_type> {};

	template <> struct TypeCaster<nullptr_t> : VoidCaster<nullptr_t> {};

	template <> struct TypeCaster<void> : TypeCaster<void_type>
	{
		using TypeCaster<void_type>::cast;

		bool load(OBJ const & src, bool)
		{
			if (src.is_null()) { return (value = nullptr), true; }

			else if (CAPSULE::check_(src)) { return (value = CAPSULE(src)), true; }
			
			else { return false; }
		}

		static OBJ cast(void const * src, ReturnValuePolicy_, OBJ const &)
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

	template <> struct TypeCaster<bool>
	{
		bool load(OBJ const & src, bool convert)
		{
			if (src == TRUE_OBJ) { return (value = true), true; }
			
			else if (src == FALSE_OBJ) { return (value = false), true; }
			
			else { return (value = src.is_valid()), true; }
		}

		static OBJ cast(bool src, ReturnValuePolicy_, OBJ const &) { return BOOL_OBJ(src); }

		TYPE_CASTER_COMMON(bool, "bool");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct StringCaster
	{
		bool load(OBJ const & src, bool convert)
		{
			if (!src) { return false; }

			else if (STR::check_(src)) { return (value = STR(src)), true; }

			else
			{
				TYPE t{ typeof(src) };

				if (t->tp_str) { return (value = t->tp_str(src)), true; }

				else if (t->tp_repr) { return (value = t->tp_repr(src)), true; }

				else { return false; }
			}
		}

		static OBJ cast(T const & src, ReturnValuePolicy_, OBJ const &) { return STR({ src }); }

		static OBJ cast(T && src, ReturnValuePolicy_, OBJ const &) noexcept { return STR({ std::move(src) }); }

		template <class U> operator U * () { return (U *)(&***value); }

		template <class U> operator U & () { return (U &)(***value); }

		template <class U> operator U && () { return (U &&)std::move(***value); }

		template <class U> using cast_op_type = ism::cast_op_type<U>;

		static constexpr auto name{ "string" };

	protected:
		STR value{};
	};

	template <class Ch, class Tr, class Al
	> struct TypeCaster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<mpl::is_char_v<Ch>>>
		: StringCaster<std::basic_string<Ch, Tr, Al>> {};

	template <> struct TypeCaster<Path> : StringCaster<String> {};

	template <> struct TypeCaster<StringName> : StringCaster<String> {};

	template <> struct TypeCaster<StringView> : StringCaster<String> {};

	template <class T> struct TypeCaster<T, std::enable_if_t<mpl::is_char_v<T>>> : TypeCaster<BasicString<T>>
	{
		TypeCaster<BasicString<T>> str_caster;

		bool load(OBJ const & src, bool convert)
		{
			return str_caster.load(src, convert);
		}

		static OBJ cast(T const * src, ReturnValuePolicy_, OBJ const &) { return STR({ src }); }

		static OBJ cast(T const src, ReturnValuePolicy_, OBJ const &) { return INT({ src }); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct RefCaster
	{
		bool load(OBJ const & src, bool)
		{
			if (!isinstance<T>(src)) { return false; }

			else { return (value = src), true; }
		}

		static OBJ cast(OBJ const & src, ReturnValuePolicy_, OBJ const &) { return src; }

		TYPE_CASTER_COMMON(T, "ref");
	};

	template <class T> struct TypeCaster<T, std::enable_if_t<is_api_v<T> && !is_base_object_v<T>>> : RefCaster<T> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ObjectCaster
	{
		bool load(OBJ const & src, bool)
		{
			if (!isinstance<T>(src)) { return false; }
			else
			{
				LoaderLifeSupport::add(src);

				return (value = src), true;
			}
		}

		static OBJ cast(T && src, ReturnValuePolicy_, OBJ const &) noexcept { return Ref<T>{ std::move(src) }; }
		static OBJ cast(T const * src, ReturnValuePolicy_, OBJ const &) { return Ref<T>{ (T *)src }; }
		static OBJ cast(T const & src, ReturnValuePolicy_, OBJ const &) { return Ref<T>{ (T *)&src }; }

		template <class U> operator U ()
		{
			using I = mpl::intrinsic_t<U>;

			if constexpr (std::is_pointer_v<U>)
			{
				if constexpr (std::is_same_v<I, Object>) { return *value; } // no cast

				else { return dynamic_cast<I *>(*value); }
			}
			else
			{
				if constexpr (std::is_same_v<I, Object>) { return *VALIDATE(*value); } // no cast

				else { return *VALIDATE(dynamic_cast<I *>(*value)); }
			}
		}

		template <class U> using cast_op_type = ism::cast_op_type<U>;

		static constexpr auto name{ "object" };

	protected:
		OBJ value{};
	};

	template <class T> struct TypeCaster<T, std::enable_if_t<is_base_object_v<T>>> : ObjectCaster<T> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// cast
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	
	template <class T, class SFINAE = void
	> struct return_policy_override { static ReturnValuePolicy_ policy(ReturnValuePolicy_ p) { return p; } };

	template <class T> struct return_policy_override<T, std::enable_if_t<std::is_base_of_v<TypeCasterGeneric, make_caster<T>>, void>>
	{
		static ReturnValuePolicy_ policy(ReturnValuePolicy_ p) { return !std::is_lvalue_reference_v<T> && !std::is_pointer_v<T> ? ReturnValuePolicy_Move : p; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE
	> auto load_type(TypeCaster<T, SFINAE> & convt, OBJ const & o) -> TypeCaster<T, SFINAE> &
	{
		if (!convt.load(o, true)) {
			CRASH("TYPE CONVERSION FAILED");
		}
		return convt;
	}

	template <class T
	> auto load_type(OBJ const & o) -> make_caster<T>
	{
		make_caster<T> convt{};
		load_type(convt, o);
		return convt;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// api -> c++
	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD T cast(OBJ const & o) { return cast_op<T>(load_type<T>(o)); }

	// api -> api
	template <class T, class O, std::enable_if_t<is_api_v<T> && is_api_v<O>, int> = 0
	> NODISCARD T cast(O && o) noexcept
	{
		if constexpr (is_base_object_v<T>)
		{
			return cast_op<T>(load_type<T>(FWD(o)));
		}
		else
		{
			return T{ FWD(o) };
		}
	}

	// c++ -> api
	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD OBJ cast(T && o, ReturnValuePolicy_ policy = ReturnValuePolicy_AutomaticReference, OBJ const & parent = {})
	{
		if (policy == ReturnValuePolicy_Automatic)
		{
			policy = (std::is_pointer_v<std::remove_reference_t<T>>
				? ReturnValuePolicy_TakeOwnership
				: (std::is_lvalue_reference_v<T>
					? ReturnValuePolicy_Copy
					: ReturnValuePolicy_Move));
		}
		else if (policy == ReturnValuePolicy_AutomaticReference)
		{
			policy = (std::is_pointer_v<std::remove_reference_t<T>>
				? ReturnValuePolicy_Reference
				: (std::is_lvalue_reference_v<T>
					? ReturnValuePolicy_Copy
					: ReturnValuePolicy_Move));
		}
		return OBJ{ make_caster<T>::cast(FWD(o), policy, parent) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> auto move(OBJ && o) -> std::enable_if_t<!move_never_v<T>, T>
	{
		if (o && o->get_ref_count() > 1) {
			ASSERT(!"Unable to cast Core instance to C++ rvalue: instance has multiple references (compile in debug mode for details)");
		}
		T ret{ std::move(load_type<T>(o).operator T & ()) };
		return ret;
	}

	template <class T
	> auto cast(OBJ && o) -> std::enable_if_t<move_always_v<T>, T>
	{
		return move<T>(std::move(o));
	}

	template <class T
	> auto cast(OBJ && o) -> std::enable_if_t<move_if_unreferenced_v<T>, T>
	{
		if (o && o->has_references())
		{
			return cast<T>(o);
		}
		else
		{
			return move<T>(std::move(o));
		}
	}

	template <class T
	> auto cast(OBJ && o) -> std::enable_if_t<move_never_v<T>, T>
	{
		return cast<T>(o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<!is_api_v<T>, int>
	> NODISCARD OBJ object_or_cast(T && o) { return ism::cast(FWD(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// ref cast
	template <class O> template <class T> inline T Ref<O>::cast() const &
	{
		if constexpr (!std::is_void_v<T>) { return ism::cast<T>(*this); }
	}

	template <class O> template <class T> inline T Ref<O>::cast() &&
	{
		if constexpr (!std::is_void_v<T>) { return ism::cast<T>(std::move(*this)); }
	}

	// object cast
	template <class T> inline T Object::cast() const &
	{
		if constexpr (!std::is_void_v<T>) { return ism::cast<T>(ptr()); }
	}

	template <class T> inline T Object::cast() &&
	{
		if constexpr (!std::is_void_v<T>) { return ism::cast<T>(std::move(*this).ptr()); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAST_HPP_
