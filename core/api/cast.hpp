#ifndef _ISM_CAST_HPP_
#define _ISM_CAST_HPP_

#include <core/api/internals.hpp>

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool isinstance_generic(OBJECT const & o, std::type_info const & t)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline std::pair<decltype(RuntimeState::_registry::registered_types_core)::iterator, bool> all_type_info_get_cache(TYPE const & t);

	inline void all_type_info_populate(TYPE const & t, Vector<TypeInfo *> & bases)
	{
		if (!isinstance<TYPE>(t) || !isinstance<LIST>(t->tp_bases)) { return; }

		Vector<TYPE> check{};
		for (OBJECT parent : ***LIST(t->tp_bases)) { check.push_back(parent); }
		
		auto const & type_dict{ get_registry().registered_types_core };
		for (size_t i = 0; i < check.size(); i++)
		{
			auto & type{ check[i] };
			if (!isinstance<TYPE>(type)) { continue; }
			if (auto it{ type_dict.find(type) }; it != type_dict.end())
			{
				for (auto * tinfo : it->second)
				{
					bool found{};
					for (auto * known : bases) { if (known == tinfo) { found = true; break; } }
					if (!found) { bases.push_back(tinfo); }
				}
			}
			else if (isinstance<LIST>(type->tp_bases))
			{
				if (i + 1 == check.size()) { check.pop_back(); i--; }

				for (OBJECT parent : ***LIST(type->tp_bases)) { check.push_back(parent); }
			}
		}
	}

	inline Vector<TypeInfo *> const & all_type_info(TYPE const & t)
	{
		auto ins = all_type_info_get_cache(t);
		if (ins.second) { all_type_info_populate(t, ins.first->second); }
		return ins.first->second;
	}

	inline TypeInfo * get_type_info(TYPE const & t)
	{
		auto & bases{ all_type_info(t) };
		if (bases.empty()) { return nullptr; }
		if (bases.size() > 1) { VERIFY(!"type has multiple bases registered"); }
		return bases.front();
	}

	inline TypeInfo * get_local_type_info(std::type_index const & t)
	{
		auto & types{ registered_local_types_cpp() };
		if (auto it{ types.find(t) }; it != types.end()) { return it->second; }
		return nullptr;
	}

	inline TypeInfo * get_global_type_info(std::type_index const & t)
	{
		auto & types{ get_registry().registered_types_cpp };
		if (auto it{ types.find(t) }; it != types.end()) { return it->second; }
		return nullptr;
	}

	NEVER_INLINE inline TypeInfo * get_type_info(std::type_index const & t, bool throw_if_missing = false)
	{
		if (auto ltype{ get_local_type_info(t) }) { return ltype; }
		if (auto gtype{ get_global_type_info(t) }) { return gtype; }
		if (throw_if_missing) { VERIFY("type not found"); }
		return nullptr;
	}

	NEVER_INLINE inline OBJECT get_type_handle(std::type_info const & t, bool throw_if_missing)
	{
		auto tpi{ get_type_info(t, throw_if_missing) };
		return tpi ? tpi->type : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct type_caster_generic
	{
		type_caster_generic(std::type_info const & info) : typeinfo{}, cpptype{ &info }, value{} {}
		type_caster_generic(TypeInfo const * tinfo) : typeinfo{ typeinfo }, cpptype{ typeinfo ? typeinfo->cpptype : nullptr } {}
		NODISCARD bool load(OBJECT const & src, bool convert) { return load_impl<type_caster_generic>(src, convert); }

		static OBJECT cast(void const * src, ReturnPolicy policy, OBJECT parent, TypeInfo const * tinfo, void * (*copy_constructor)(void const *), void * (*move_constructor)(void const *), void const * existing_holder = nullptr)
		{
			if (!tinfo) { return nullptr; }
			if (!src) { return Core_None; }

			return nullptr;
		}

		bool try_implicit_casts(OBJECT src, bool convert)
		{
			for (auto & cast : typeinfo->implicit_casts)
			{
				if (type_caster_generic sub_caster{ *cast.first }; sub_caster.load(src, convert))
				{
					value = cast.second(sub_caster.value);
					return true;
				}
			}
			return false;
		}

		bool try_direct_conversions(OBJECT src)
		{
			for (auto & converter : *typeinfo->direct_conversions)
			{
				if (converter(src.ptr(), value))
				{
					return true;
				}
			}
			return false;
		}

		void check_holder_compat() {}
		
		static void * local_load(OBJECT src, TypeInfo const * ti)
		{
			type_caster_generic caster{ ti };
			if (caster.load(src, false)) { return caster.value; }
			return nullptr;
		}

		bool try_load_foreign_module_local(OBJECT src)
		{
			constexpr auto local_key{ "__module_local__" };
			TYPE ptype{ typeof(src) };
			if (!hasattr(ptype, local_key)) { return false; }
			auto foreign_typeinfo{ CAPSULE(ptype.attr(local_key))->get_pointer<TypeInfo *>() };
			if (foreign_typeinfo->module_local_load == &local_load || (cpptype && !same_type(*cpptype, *foreign_typeinfo->cpptype)))
			{
				return false;
			}
			if (auto result{ foreign_typeinfo->module_local_load(src, foreign_typeinfo) })
			{
				value = result;
				return true;
			}
			return false;
		}

		template <class ThisT> bool load_impl(OBJECT const & src, bool convert)
		{
			if (!src) { return false; }
			// TODO...
			return false;
		}

		static std::pair<const void *, TypeInfo const *> src_and_type(void const * src, std::type_info const & cast_type, std::type_info const * rtti_type = {})
		{
			if (auto tpi{ get_type_info(cast_type) }) { return { src, const_cast<TypeInfo const *>(tpi) }; }
			return { nullptr, nullptr };
		}

		TypeInfo const * typeinfo{};
		std::type_info const * cpptype{};
		void * value{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> using cast_op_type =
		std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
		typename std::add_pointer_t<intrinsic_t<T>>,
		typename std::add_lvalue_reference_t<intrinsic_t<T>>>;

	template <class T
	> using movable_cast_op_type =
		std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
		typename std::add_pointer_t<intrinsic_t<T>>,
		std::conditional_t<std::is_rvalue_reference_v<T>,
		typename std::add_rvalue_reference_t<intrinsic_t<T>>,
		typename std::add_lvalue_reference_t<intrinsic_t<T>>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename SFINAE = void
	> struct is_copy_constructible : std::is_copy_constructible<T> {};

	template <class Container
	> struct is_copy_constructible<Container, std::enable_if_t<all_of_v<
		std::is_copy_constructible<Container>,
		std::is_same<typename Container::value_type &,
		typename Container::reference>,
		std::negation<std::is_same<Container, typename Container::value_type>>
		>>> : is_copy_constructible<typename Container::value_type> {};

	template <class T1, class T2
	> struct is_copy_constructible<std::pair<T1, T2>> : all_of<is_copy_constructible<T1>, is_copy_constructible<T2>> {};

	template <class T
	> constexpr bool is_copy_constructible_v{ is_copy_constructible<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void
	> struct is_copy_assignable : std::is_copy_assignable<T> {};

	template <class Container
	> struct is_copy_assignable<Container, std::enable_if_t<all_of_v<
		std::is_copy_assignable<Container>,
		std::is_same<typename Container::value_type &, typename Container::reference>
		>>> : is_copy_assignable<typename Container::value_type> {};
	
	template <class T1, class T2
	> struct is_copy_assignable<std::pair<T1, T2>> : all_of<is_copy_assignable<T1>, is_copy_assignable<T2>> {};

	template <class T
	> constexpr bool is_copy_assignable_v{ is_copy_assignable<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class itype, class SFINAE = void
	> struct polymorphic_type_hook_base
	{
		static void const * get(itype const * src, std::type_info const *&) { return src; }
	};

	template <class itype
	> struct polymorphic_type_hook_base<itype, std::enable_if_t<std::is_polymorphic_v<itype>>>
	{
		static void const * get(itype const * src, std::type_info const *& type) {
			type = src ? &typeid(*src) : nullptr;
			return dynamic_cast<void const *>(src);
		}
	};

	template <class itype, class SFINAE = void
	> struct polymorphic_type_hook : public polymorphic_type_hook_base<itype> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct type_caster_base : type_caster_generic
	{
		using itype = intrinsic_t<T>;

		type_caster_base() : type_caster_base{ typeid(T) } {}
		explicit type_caster_base(std::type_info const & info) : type_caster_generic{ info } {}

		static OBJECT cast(itype const & src, ReturnPolicy policy, OBJECT parent) {
			return cast(&src, policy, parent);
		}

		static OBJECT cast(itype && src, ReturnPolicy policy, OBJECT parent) {
			return cast(&src, ReturnPolicy::Move, parent);
		}

		static std::pair<const void *, TypeInfo const *> src_and_type(itype const * src)
		{
			auto & cast_type{ typeid(itype) };
			const std::type_info * instance_type{};
			const void * vsrc{ polymorphic_type_hook<itype>::get(src, instance_type) };
			if (instance_type && !same_type(cast_type, *instance_type))
			{
				// This is a base pointer to a derived type. If the derived type is registered
				// with pybind11, we want to make the full derived object available.
				// In the typical case where itype is polymorphic, we get the correct
				// derived pointer (which may be != base pointer) by a dynamic_cast to
				// most derived type. If itype is not polymorphic, we won't get here
				// except via a user-provided specialization of polymorphic_type_hook,
				// and the user has promised that no this-pointer adjustment is
				// required in that case, so it's OK to use static_cast.
				if (auto const * tpi{ get_type_info(*instance_type) }) { return { vsrc, tpi }; }
			}
			// nullptr, an `itype` pointer, or an unknown derived pointer, so don't do a cast
			return type_caster_generic::src_and_type(src, cast_type, instance_type);
		}

		static OBJECT cast(itype const * src, ReturnPolicy policy, OBJECT parent)
		{
			auto st{ src_and_type(src) };
			return type_caster_generic::cast(st.first, policy, parent, st.second, make_copy_constructor(src), make_move_constructor(src));
		}

		static OBJECT cast_holder(itype const * src, void const * holder)
		{
			auto st{ src_and_type(src) };
			return type_caster_generic::cast(st.first, ReturnPolicy::TakeOwnership, {}, st.second, nullptr, nullptr, holder);
		}

		template <typename T> using cast_op_type = detail::cast_op_type<T>;

		operator itype * () { return (T *)value; }
		operator itype & () { return *((itype *)CHECK(value)); }

	protected:
		using Constructor = void * (*)(void const *);

		template <class T, class = std::enable_if_t<is_copy_constructible_v<T>>
		> static auto make_copy_constructor(T const * x) -> decltype(new T(*x), Constructor{})
		{
			return [](void const * arg) -> void * { return memnew(T(*reinterpret_cast<T const *>(arg))); };
		}

		template <class T, class = std::enable_if_t<std::is_move_constructible_v<T>>
		> static auto make_move_constructor(T const * x) -> decltype(new T(std::move(*const_cast<T *>(x))), Constructor{})
		{
			return [](void const * arg) -> void * { return memnew(T(std::move(*const_cast<T *>(reinterpret_cast<T const *>(arg))))); };
		}

		static Constructor make_copy_constructor(...) { return nullptr; }
		static Constructor make_move_constructor(...) { return nullptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct type_caster : type_caster_base<T> {};

	template <class T> using make_caster = typename type_caster<intrinsic_t<T>>;

	template <class T> auto cast_op(make_caster<T> & caster) -> typename make_caster<T>::template cast_op_type<T>
	{
		return caster.operator typename make_caster<T>::template cast_op_type<T>();
	}

	template <class T> auto cast_op(make_caster<T> && caster) -> typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>
	{
		return std::move(caster).operator typename make_caster<T>::template cast_op_type<typename std::add_rvalue_reference_t<T>>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_TYPE_CASTER(m_type, m_name) \
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
		template <class T> using cast_op_type = _ISM detail::movable_cast_op_type<T>; \

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

		ISM_TYPE_CASTER(T, std::is_integral_v<T> ? "int" : "float");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct void_caster
	{
		NODISCARD bool load(OBJECT const & src, bool) { return (src.is_valid() && src.is_none()); }

		NODISCARD static OBJECT cast(T, ReturnPolicy, OBJECT) { return Core_None; }

		ISM_TYPE_CASTER(T, "none");
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

	template <> struct type_caster<nullptr_t> : void_caster<nullptr_t> {};

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

		ISM_TYPE_CASTER(bool, "bool");
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

		ISM_TYPE_CASTER(T, "string");
	};

	template <class Ch, class Tr, class Al
	> struct type_caster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<is_char_v<Ch>>>
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

	template <class T, class Holder
	> struct copyable_holder_caster : type_caster_base<T>
	{
		// for holder types like std::shared_ptr, etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct object_caster
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

		ISM_TYPE_CASTER(T, "object");
	};

	template <class T> struct type_caster<T, std::enable_if_t<is_object_api_v<T>>>
		: object_caster<T> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// move_is_plain_type
	template <class T> using move_is_plain_type = satisfies_none_of<T,
		std::is_void,
		std::is_pointer,
		std::is_reference,
		std::is_const>;
	template <class T> constexpr bool move_is_plain_type_v{ move_is_plain_type<T>::value };

	// move_always
	template <class T, class SFINAE = void> struct move_always : std::false_type {};
	template <class T> struct move_always<T, std::enable_if_t<all_of_v<
		move_is_plain_type<T>,
		std::negation<is_copy_constructible<T>>,
		std::is_move_constructible<T>,
		std::is_same<decltype(std::declval<make_caster<T>>().operator T & ()), T &>
	>>> : std::true_type {};
	template <class T> constexpr bool move_always_v{ move_always<T>::value };

	// move_if_unreferenced
	template <class T, class SFINAE = void> struct move_if_unreferenced : std::false_type {};
	template <class T> struct move_if_unreferenced<T, std::enable_if_t<all_of_v<
		move_is_plain_type<T>,
		std::negation<move_always<T>>,
		std::is_move_constructible<T>,
		std::is_same<decltype(std::declval<make_caster<T>>().operator T & ()), T &>
	>>> : std::true_type {};
	template <class T> constexpr bool move_if_unreferenced_v{ move_if_unreferenced<T>::value };

	// move_never
	template <class T> using move_never = none_of<move_always<T>, move_if_unreferenced<T>>;
	template <class T> constexpr bool move_never_v{ move_never<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Return, class SFINAE = void
	> struct return_policy_override { static ReturnPolicy policy(ReturnPolicy p) { return p; } };

	template <class Return
	> struct return_policy_override<Return, std::enable_if_t<std::is_base_of_v<type_caster_generic, make_caster<Return>>, void>>
	{
		static ReturnPolicy policy(ReturnPolicy p)
		{
			return !std::is_lvalue_reference_v<Return> && !std::is_pointer_v<Return> ? ReturnPolicy::Move : p;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE
	> auto load_type(type_caster<T, SFINAE> & conv, OBJECT const & o) -> type_caster<T, SFINAE> &
	{
		VERIFY(conv.load(o, true));
		return conv;
	}

	template <typename T
	> auto load_type(OBJECT const & o) -> make_caster<T>
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
		}
		else if (policy == ReturnPolicy::AutomaticReference) {
			policy = (std::is_pointer_v<U>
				? ReturnPolicy::Reference
				: (std::is_lvalue_reference_v<T>
					? ReturnPolicy::Copy
					: ReturnPolicy::Move));
		}

		return detail::make_caster<T>::cast(FWD(o), policy, parent);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> auto move(OBJECT && o) -> std::enable_if_t<!detail::move_never_v<T>, T>
	{
		if (o && o->ref_count() > 1) {
			VERIFY(!"Unable to cast Core instance to C++ rvalue: instance has multiple references (compile in debug mode for details)");
		}
		T ret{ std::move(detail::load_type<T>(o).operator T & ()) };
		return ret;
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<detail::move_always<T>::value, T>
	{
		return _ISM move<T>(std::move(o));
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<detail::move_if_unreferenced<T>::value, T>
	{
		if (o && o->ref_count() > 1) return _ISM cast<T>(o);
		else return _ISM move<T>(std::move(o));
	}

	template <class T
	> auto cast(OBJECT && o) -> std::enable_if_t<detail::move_never_v<T>, T>
	{
		return _ISM cast<T>(o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O> template <class T> inline T Handle<O>::cast() const &
	{
		if constexpr (!std::is_same_v<T, void>) { return _ISM cast<T>(*this); }
	}

	template <class O> template <class T> inline T Handle<O>::cast() &&
	{
		if constexpr (!std::is_same_v<T, void>) { return _ISM cast<T>(std::move(*this)); }
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int>
	> NODISCARD OBJECT object_or_cast(T && o) { return _ISM cast(FWD(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FunctionRecord;

	struct FunctionCall
	{
		FunctionCall(FunctionRecord const & func, OBJECT parent);

		FunctionRecord const & func;

		Vector<OBJECT> args{};

		Vector<bool> args_convert{};

		OBJECT parent{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct argument_loader
	{
	public:
		static constexpr size_t nargs{ sizeof...(Args) };

		using indices = std::make_index_sequence<nargs>;

		std::tuple<detail::make_caster<Args>...> argcasters;

		bool load_args(FunctionCall & call)
		{
			return impl_load_sequence(call, indices{});
		}

		template <class Return, class Guard, class F
		> auto call(F && f) && -> std::conditional_t<std::is_void_v<Return>, void_type, Return>
		{
			if constexpr (std::is_void_v<Return>)
			{
				std::move(*this).template call_impl<Return>(FWD(f), indices{}, Guard{});
				return void_type{};
			}
			else
			{
				return std::move(*this).template call_impl<Return>(FWD(f), indices{}, Guard{});
			}
		}

	private:
		static bool impl_load_sequence(FunctionCall &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_sequence(FunctionCall & call, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(call.args[I], call.args_convert[I]));
		}

		template <class R, class F, size_t ... I, class Guard
		> R call_impl(F && f, std::index_sequence<I...>, Guard &&) &&
		{
			return f(detail::cast_op<Args>(std::move(std::get<I>(argcasters)))...);
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

	inline OBJECT call_object(OBJECT callable, OBJECT args)
	{
		if (vectorcallfunc func{ _get_vectorcall(callable) })
		{
			auto list{ (CoreList *)args.ptr() };
			return func(callable, list->data(), list->size());
		}
		else if (TYPE t{ typeof(callable) }; t->tp_call)
		{
			return t->tp_call(callable, args);
		}
		else
		{
			return nullptr;
		}
	}

	template <ReturnPolicy policy
	> struct argument_collector
	{
		template <class ... Args
		> explicit argument_collector(Args && ... values) : m_args{ LIST::create() }
		{
			m_args->reserve(sizeof...(Args));

			mpl::for_args([&](auto && e) noexcept
			{
				m_args->append(detail::make_caster<decltype(e)>::cast(FWD(e), policy, nullptr));
			}
			, FWD(values)...);
		}

		LIST const & args() const & { return m_args; }

		LIST args() && { return std::move(m_args); }

		OBJECT call(OBJECT callable) { return call_object(callable, m_args); }

	private:
		LIST m_args{};
	};

	template <ReturnPolicy policy, class ... Args
	> auto collect_arguments(Args && ... args) noexcept
	{
		return argument_collector<policy>{ FWD(args)... };
	}

	template <class Derived
	> template <ReturnPolicy policy, class ...Args
	> OBJECT ObjectAPI<Derived>::operator()(Args && ... args)
	{
		return collect_arguments<policy>(FWD(args)...).call(handle());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAST_HPP_
