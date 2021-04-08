#ifndef _ISM_RUNTIME_HPP_
#define _ISM_RUNTIME_HPP_

#include <core/api/callable.hpp>

#define ISM_INTERNALS_ID "__ism_internals__"
#define ISM_MODULE_LOCAL_ID "__ism_module_local__"

// core types
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _CoreTag {};

	template <class T> constexpr bool is_core_v{ std::is_base_of_v<_CoreTag, intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class CoreObject;
	class CoreType;
	class CoreInt;
	class CoreFloat;
	class CoreString;
	class CoreList;
	class CoreDict;
	class CoreCapsule;
	class CoreMethod;
	class CoreGeneric;
	class CoreModule;

	template <class T> struct Handle;
	ALIAS(OBJECT)	Handle<CoreObject>;
	ALIAS(TYPE)		Handle<CoreType>;
	ALIAS(INT)		Handle<CoreInt>;
	ALIAS(FLT)		Handle<CoreFloat>;
	ALIAS(STR)		Handle<CoreString>;
	ALIAS(LIST)		Handle<CoreList>;
	ALIAS(DICT)		Handle<CoreDict>;
	ALIAS(CAPSULE)	Handle<CoreCapsule>;
	ALIAS(METHOD)	Handle<CoreMethod>;
	ALIAS(GENERIC)	Handle<CoreGeneric>;
	ALIAS(MODULE)	Handle<CoreModule>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy> struct Accessor;
	namespace ACCESS
	{
		template <class T> struct AttrPolicy;
		template <class T> struct ItemPolicy;
	}
	template <class T> ALIAS(AttrAccessor) Accessor<ACCESS::AttrPolicy<T>>;
	template <class T> ALIAS(ItemAccessor) Accessor<ACCESS::ItemPolicy<T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// enums
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum DataType_ : int32_t
	{
		DataType_Invalid = -1,
		DataType_Int8, DataType_Int16, DataType_Int32, DataType_Int64,
		DataType_Uint8, DataType_Uint16, DataType_Uint32, DataType_Uint64,
		DataType_Float, DataType_Double,
		DataType_String, DataType_Object,
		DataType_Char, DataType_Byte, DataType_Bool,
		DataType_None,

#if ISM_ARCH < 64
		DataType_SizeT = DataType_Uint32,
		DataType_SSizeT = DataType_Int32,
#else
		DataType_SizeT = DataType_Uint64,
		DataType_SSizeT = DataType_Int64,
#endif
	}
	DataType;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class return_value_policy : uint8_t
	{
		automatic = 0,
		automatic_reference,
		take_ownership,
		copy,
		move,
		reference,
		reference_internal
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// method types
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		StdFn<OBJECT(OBJECT a)>;
	ALIAS(binaryfunc)		StdFn<OBJECT(OBJECT a, OBJECT b)>;
	ALIAS(ternaryfunc)		StdFn<OBJECT(OBJECT a, OBJECT b, OBJECT c)>;
	ALIAS(inquiry)			StdFn<bool(OBJECT o)>;
	ALIAS(lenfunc)			StdFn<ssize_t(OBJECT o)>;

	ALIAS(getattrfunc)		StdFn<OBJECT(OBJECT self, cstring name)>;
	ALIAS(setattrfunc)		StdFn<Err(OBJECT self, cstring name, OBJECT value)>;

	ALIAS(getattrofunc)		StdFn<OBJECT(OBJECT o, OBJECT name)>;
	ALIAS(setattrofunc)		StdFn<Err(OBJECT o, OBJECT name, OBJECT value)>;

	ALIAS(descrgetfunc)		StdFn<OBJECT(OBJECT, OBJECT, OBJECT)>;
	ALIAS(descrsetfunc)		StdFn<Err(OBJECT, OBJECT, OBJECT)>;

	ALIAS(cmpfunc)			StdFn<int32_t(OBJECT a, OBJECT b)>;
	ALIAS(hashfunc)			StdFn<hash_t(OBJECT o)>;
	ALIAS(reprfunc)			StdFn<STR(OBJECT o)>;

	ALIAS(allocfunc)		StdFn<void * (size_t size)>;
	ALIAS(freefunc)			StdFn<void(void * ptr)>;
	ALIAS(createfunc)		StdFn<OBJECT()>;

	ALIAS(cfunction)		StdFn<OBJECT(OBJECT self, OBJECT args)>;
	ALIAS(vectorcallfunc)	StdFn<OBJECT(OBJECT callable, OBJECT const * args, size_t nargs)>;

	ALIAS(getter)			StdFn<OBJECT(OBJECT self, void * context)>;
	ALIAS(setter)			StdFn<Err(OBJECT self, OBJECT value, void * context)>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD AsNumber final
	{
		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(AsNumber);

		binaryfunc	operator_add{};
		binaryfunc	operator_subtract{};
		binaryfunc	operator_multiply{};
		binaryfunc	operator_divide{};
		binaryfunc	operator_remainder{};
		ternaryfunc	operator_power{};
		binaryfunc	operator_lshift{};
		binaryfunc	operator_rshift{};
		binaryfunc	operator_and{};
		binaryfunc	operator_xor{};
		binaryfunc	operator_or{};

		binaryfunc	operator_inplace_add{};
		binaryfunc	operator_inplace_subtract{};
		binaryfunc	operator_inplace_multiply{};
		binaryfunc	operator_inplace_divide{};
		binaryfunc	operator_inplace_remainder{};
		ternaryfunc	operator_inplace_power{};
		binaryfunc	operator_inplace_lshift{};
		binaryfunc	operator_inplace_rshift{};
		binaryfunc	operator_inplace_and{};
		binaryfunc	operator_inplace_xor{};
		binaryfunc	operator_inplace_or{};

		unaryfunc	operator_positive{};
		unaryfunc	operator_negative{};
		unaryfunc	operator_absolute{};
		inquiry		operator_bool{};
		unaryfunc	operator_invert{};
		unaryfunc	operator_integer{};
		unaryfunc	operator_decimal{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD AsSequence final
	{
		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(AsSequence);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD AsMapping final
	{
		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(AsMapping);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum MethodFlags_ : int32_t
	{
		MethodFlags_None,
		MethodFlags_VarArgs = 1 << 0,
		MethodFlags_Keywords = 1 << 1,
		MethodFlags_NoArgs = 1 << 2,
		MethodFlags_O = 1 << 3,
		MethodFlags_Class = 1 << 4,
		MethodFlags_Static = 1 << 5,
		MethodFlags_Coexist = 1 << 6,
		MethodFlags_FastCall = 1 << 7,
		MethodFlags_Stackless = 1 << 8,
		MethodFlags_Method = 1 << 9,
	}
	MethodFlags;

	class NODISCARD MethodDef
	{
	public:
		String		name{};
		cfunction	method{};
		int32_t		flags{ MethodFlags_None };
		String		doc{};

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(MethodDef);

		template <class V> static auto find_in(V & v, String const & i) -> MethodDef *
		{
			if (auto it{ std::find_if(v.begin(), v.end(), [&i
			](MethodDef const & e) { return e.name == i; }) }
			; it != v.end()) {
				return std::addressof(*it);
			}
			return nullptr;
		}
	};

	template <> struct ISM::Hash<MethodDef>
	{
		hash_t operator()(MethodDef const & a) const { return hashof(a.name); }
	};

	template <> struct ISM::EqualTo<MethodDef>
	{
		bool operator()(MethodDef const & a, MethodDef const & b) const { return &a == &b || a.name == b.name; }

		bool operator()(MethodDef const & a, String const & i) const { return a.name == i; }
	};

	template <> struct ISM::Less<MethodDef>
	{
		bool operator()(MethodDef const & a, MethodDef const & b) const { return &a != &b && a.name < b.name; }

		bool operator()(MethodDef const & a, String const & i) const { return a.name < i; }
	};

	template <class V> MethodDef * _Find_Method(V & v, String const & i)
	{
		if (auto it{ std::find_if(v.begin(), v.end(), [&i
		](MethodDef const & e) { return e.name == i; }) }
		; it != v.end()) { return std::addressof(*it); }
		else { return nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD GetSetDef
	{
	public:
		String	name{};
		getter	get{};
		setter	set{};
		void *	closure{};
		String	doc{};

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(GetSetDef);

		template <class V> static auto find_in(V & v, String const & i) -> GetSetDef *
		{
			if (auto it{ std::find_if(v.begin(), v.end(), [&i
			](GetSetDef const & e) { return e.name == i; }) }
			; it != v.end()) {
				return std::addressof(*it);
			}
			return nullptr;
		}
	};

	template <> struct ISM::Hash<GetSetDef>
	{
		hash_t operator()(GetSetDef const & a) const { return hashof(a.name); }
	};

	template <> struct ISM::EqualTo<GetSetDef>
	{
		bool operator()(GetSetDef const & a, GetSetDef const & b) const { return &a == &b || a.name == b.name; }

		bool operator()(GetSetDef const & a, String const & i) const { return a.name == i; }
	};

	template <> struct ISM::Less<GetSetDef>
	{
		bool operator()(GetSetDef const & a, GetSetDef const & b) const { return &a != &b && a.name < b.name; }

		bool operator()(GetSetDef const & a, String const & i) const { return a.name < i; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object api
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : _CoreTag
	{
	private:
		NODISCARD auto derived() const noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		NODISCARD auto handle() const noexcept
		{
			if constexpr (std::is_base_of_v<CoreObject, Derived>)
			{
				return Handle<Derived>{ derived().ptr() };
			}
			else
			{
				return derived();
			}
		}

		NODISCARD auto hash() const noexcept
		{
			TYPE t{ CHECK(type()) };
			return t->tp_hash ? t->tp_hash(handle()) : hash_t{};
		}

		NODISCARD auto len() const noexcept
		{
			TYPE t{ CHECK(type()) };
			return t->tp_len ? t->tp_len(handle()) : size_t{};
		}

		NODISCARD auto repr() const noexcept
		{
			TYPE t{ CHECK(type()) };
			return t->tp_repr ? t->tp_repr(handle()) : STR{};
		}
		
		NODISCARD auto str() const noexcept
		{
			TYPE t{ CHECK(type()) };
			return t->tp_str ? t->tp_str(handle()) : STR{};
		}

		NODISCARD auto type() const noexcept
		{
			return is_valid() ? derived().ptr()->ob_type : TYPE{};
		}

		template <class U = Derived
		> NODISCARD auto compare(ObjectAPI<U> const & o) const -> int32_t
		{
			if (TYPE t{ CHECK(type()) }; t->tp_compare)
			{
				return t->tp_compare(handle(), o.handle());
			}
			else
			{
				return UTIL::compare(derived().ptr(), o.derived().ptr());
			}
		}

		template <class I = cstring
		> NODISCARD auto attr(I && i) const { return AttrAccessor<I>{ handle(), FWD(i) }; }

		template <class I = cstring
		> NODISCARD auto operator[](I && i) const { return ItemAccessor<I>{ handle(), FWD(i) }; }

		template <class O = OBJECT
		> NODISCARD bool contains(O && v) const { return this->attr("__contains__")(FWD(v)); }

		NODISCARD bool check(TYPE const & value) const noexcept { return type().is(value); }
		NODISCARD bool is(ObjectAPI const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }
		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }
		NODISCARD bool equal_to(ObjectAPI const & o) const noexcept { return compare(o) == 0; }
		NODISCARD bool not_equal_to(ObjectAPI const & o) const noexcept { return compare(o) != 0; }
		NODISCARD bool less(ObjectAPI const & o) const noexcept { return compare(o) < 0; }
		NODISCARD bool less_equal(ObjectAPI const & o) const noexcept { return compare(o) <= 0; }
		NODISCARD bool greater(ObjectAPI const & o) const noexcept { return compare(o) > 0; }
		NODISCARD bool greater_equal(ObjectAPI const & o) const noexcept { return compare(o) >= 0; }

		NODISCARD auto doc() const { return this->attr("__doc__"); }

		template <class ... Args
		> OBJECT operator()(Args && ... args);
	};

	template <class U
	> std::ostream & operator<<(std::ostream & out, ObjectAPI<U> const & value)
	{
		TYPE t{ CHECK(value.type()) };
		auto s{ (t->tp_str ? t->tp_str(value.handle()) : (t->tp_repr ? t->tp_repr(value.handle()) : STR{})) };
		if (s) { out << s.cast<String>(); }
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// handle
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Handle : public ObjectAPI<Handle<T>>, public Ref<T>
	{
	public:
		using self_type = Handle<T>;
		using base_type = Ref<T>;

		using typename base_type::element_type;
		using typename base_type::pointer;

		using base_type::Ref;
		using base_type::operator=;

		virtual ~Handle() override = default;
		NODISCARD static auto type_static() noexcept { return ISM::typeof<T>(); }
		template <class T> NODISCARD T cast() const &;
		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// comparators
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class U
	> NODISCARD bool operator==(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.equal_to(b); }

	template <class T, class U
	> NODISCARD bool operator!=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.not_equal_to(b); }

	template <class T, class U
	> NODISCARD bool operator<(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.less(b); }

	template <class T, class U
	> NODISCARD bool operator<=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.less_equal(b); }

	template <class T, class U
	> NODISCARD bool operator>(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.greater(b); }

	template <class T, class U
	> NODISCARD bool operator>=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ISM::Hash<Handle<T>>
	{
		NODISCARD hash_t operator()(Handle<T> const & o) const { return o.hash(); }
	};

	template <class T> struct ISM::EqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct ISM::NotEqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct ISM::Less<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct ISM::Greater<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct ISM::LessEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct ISM::GreaterEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// helper functions
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace IMPL
	{
		template <class T, class SFINAE = void
		> struct handle_type { using type = OBJECT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<std::is_integral_v<T>>> { using type = INT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<std::is_floating_point_v<T>>> { using type = FLT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<is_string_v<T>>> { using type = STR; };
	}

	template <class T> ALIAS(make_handle) typename IMPL::handle_type<intrinsic_t<T>>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_core_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_core_v<T>, int> = 0
	> NODISCARD OBJECT object_or_cast(T && o);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O> NODISCARD OBJECT * _get_dict_ptr(O o)
	{
		TYPE t{ o.type() };
		auto dictoffset{ t->tp_dictoffset };
		if (dictoffset == 0) { return nullptr; }
		VERIFY(0 < dictoffset);
		return (OBJECT *)((char *)o.ptr() + dictoffset);
	}

	template <class O> NODISCARD OBJECT * _get_weaklist_ptr(O o)
	{
		TYPE t{ o.type() };
		auto weaklistoffset{ t->tp_weaklistoffset };
		if (weaklistoffset == 0) { return nullptr; }
		VERIFY(0 < weaklistoffset);
		return (OBJECT *)((char *)o.ptr() + weaklistoffset);
	}

	template <class O> NODISCARD OBJECT * _get_vectorcall_ptr(O o)
	{
		TYPE t{ o.type() };
		auto vectorcalloffset{ t->tp_vectorcalloffset };
		if (vectorcalloffset == 0) { return nullptr; }
		VERIFY(0 < vectorcalloffset);
		return (OBJECT *)((char *)o.ptr() + vectorcalloffset);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_core_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return T::type_static();
	}

	template <class T, std::enable_if_t<!is_core_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return ISM::typeof<make_handle<T>>();
	}

	template <class T, std::enable_if_t<is_core_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return o.type();
	}

	template <class T, std::enable_if_t<!is_core_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return ISM::typeof(ISM::object_or_cast(FWD(o)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_core_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return o.check(ISM::typeof<T>());
	}

	template <class T, std::enable_if_t<!is_core_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return ISM::isinstance<make_handle<T>>(o);
	}

	template <class A, class B = A
	> NODISCARD inline bool isinstance(A const & o, B const & t)
	{
		return ISM::isinstance(o, ISM::isinstance<TYPE>(o) ? t : t.type());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// accessors
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy
	> struct Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using key_type = typename Policy::key_type;

		Accessor(OBJECT obj, key_type key) : m_obj{ obj }, m_key{ std::move(key) } {}
		Accessor(Accessor const &) = default;
		Accessor(Accessor &&) = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT{ a }); }
		void operator=(Accessor const & a) & { operator=(OBJECT{ a }); }

		template <class T> void operator=(T && value) && { Policy::set(m_obj, m_key, ISM::object_or_cast(FWD(value))); }
		template <class T> void operator=(T && value) & { get_cache() = ISM::object_or_cast(FWD(value)); }

		NODISCARD auto ptr() const -> CoreObject * { return get_cache().ptr(); }

		template <class T
		> NODISCARD auto cast() const -> T { return get_cache().template cast<T>(); }

		template <class O = CoreObject
		> NODISCARD operator Handle<O>() const { return get_cache(); }

	protected:
		OBJECT & get_cache() const { if (!m_cache) { m_cache = Policy::get(m_obj, m_key); } return m_cache; }

	private:
		OBJECT m_obj;
		key_type m_key;
		mutable OBJECT m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace ACCESS
	{
		template <class T> struct AttrPolicy
		{
			using key_type = T;
			
			static auto get(OBJECT o, T && i) { return ISM::getattr(o, FWD(i)); }
			
			template <class V = OBJECT
			> static void set(OBJECT o, T && i, V && v) { ISM::setattr(o, FWD(i), FWD(v)); }
		};

		template <class T> struct ItemPolicy
		{
			using key_type = T;
			
			static auto get(OBJECT o, T && i) { return ISM::getitem(o, FWD(i)); }
			
			template <class V = OBJECT
			> static void set(OBJECT o, T && i, V && v) { ISM::setitem(o, FWD(i), FWD(v)); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreObject_Type;

	class NODISCARD ISM_API CoreObject : public ObjectAPI<CoreObject>, public Reference
	{
	private:
		friend bool predelete_handler(CoreObject *);
		friend void postinitialize_handler(CoreObject *);
		void _postinitialize();
		bool _predelete();

	public:
		using base_type = typename Reference;
		using self_type = typename CoreObject;
		TYPE ob_type;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreObject_Type }; }

	public:
		virtual ~CoreObject();
		explicit CoreObject(TYPE const & t);
		CoreObject() : self_type{ type_static() } {}
		CoreObject(self_type const &) = default;
		CoreObject(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
		NODISCARD auto ptr() const noexcept { return const_cast<CoreObject *>(this); }
	};

	NODISCARD inline OBJECT object_or_cast(CoreObject * o) { return OBJECT{ o }; }

	FORCE_INLINE void ISM::postinitialize_handler(CoreObject * value) { value->_postinitialize(); }

	FORCE_INLINE bool ISM::predelete_handler(CoreObject * value) { return value->_predelete(); }

	template <class O = CoreObject, class ... Args
	> auto make_object(Args && ... args)
	{
		if constexpr (std::is_base_of_v<CoreObject, O>)
		{
			return Handle<O>{ memnew(O{ FWD(args)... }) };
		}
		else if constexpr (is_ref_v<O>)
		{
			return ISM::make_object<typename O::element_type>(FWD(args)...);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// type
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum TypeFlags_ : int32_t
	{
		TypeFlags_None = 0,

		TypeFlags_HeapType			 = 1 << 0,
		TypeFlags_BaseType			 = 1 << 1,
		TypeFlags_HaveVectorCall	 = 1 << 2,
		TypeFlags_Ready				 = 1 << 3,
		TypeFlags_Readying			 = 1 << 4,
		TypeFlags_HaveGc			 = 1 << 5,
		TypeFlags_MethodDescriptor	 = 1 << 6,
		TypeFlags_IsAbstract		 = 1 << 7,

		TypeFlags_Int_Subclass		= 1 << 25,
		TypeFlags_Float_Subclass	= 1 << 26,
		TypeFlags_Str_Subclass		= 1 << 27,
		TypeFlags_List_Subclass		= 1 << 28,
		TypeFlags_Dict_Subclass		= 1 << 29,
		TypeFlags_Type_Subclass		= 1 << 30,

		TypeFlags_Default = TypeFlags_None,
	}
	TypeFlags;

	ISM_API_DATA(CoreType) _CoreType_Type;

	class NODISCARD ISM_API CoreType : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreType;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreType_Type }; }

	public:
		virtual ~CoreType() override;
		explicit CoreType(TYPE const & t) : base_type{ t } {}
		CoreType() : base_type{ type_static() } {}
		CoreType(self_type const &) = default;
		CoreType(self_type &&) noexcept = default;
		CoreType & operator=(self_type const &) = default;
		CoreType & operator=(self_type &&) noexcept = default;

	public:
		RTTI::Info const *	tp_rtti;
		String				tp_name;
		ssize_t				tp_basicsize;
		int32_t				tp_flags;
		String				tp_doc;

		ssize_t				tp_dictoffset;
		ssize_t				tp_vectorcalloffset;
		ssize_t				tp_weaklistoffset;

		Optl<AsNumber>		tp_as_number;
		Optl<AsSequence>	tp_as_sequence;
		Optl<AsMapping>		tp_as_mapping;

		Vector<GetSetDef>	tp_getsets;
		Vector<MethodDef>	tp_methods;

		ternaryfunc			tp_call;
		cmpfunc				tp_compare;
		hashfunc			tp_hash;
		lenfunc				tp_len;
		reprfunc			tp_repr;
		reprfunc			tp_str;

		getattrfunc			tp_getattr;
		setattrfunc			tp_setattr;
		getattrofunc		tp_getattro;
		setattrofunc		tp_setattro;
		descrgetfunc		tp_descrget;
		descrsetfunc		tp_descrset;

		TYPE				tp_base;
		OBJECT				tp_bases;
		OBJECT				tp_cache;
		createfunc			tp_create;
		OBJECT				tp_dict;
		OBJECT				tp_mro;
		OBJECT				tp_subclasses;
		OBJECT				tp_weaklist;
		vectorcallfunc		tp_vectorcall;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// constant types
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreNone_Type;
	ISM_API_DATA(CoreObject)_Core_None;
#define CoreNone_Type		(ISM::TYPE{ &ISM::_CoreNone_Type })
#define Core_None			(ISM::OBJECT{ &ISM::_Core_None })

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreBool_Type;
	ISM_API_DATA(CoreInt)	_Core_True;
	ISM_API_DATA(CoreInt)	_Core_False;
#define CoreBool_Type		(ISM::TYPE{ &ISM::_CoreBool_Type })
#define Core_True			(ISM::INT{ &ISM::_Core_True })
#define Core_False			(ISM::INT{ &ISM::_Core_False })
#define Core_Boolean(b)		((b) ? Core_True : Core_False)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// int
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreInt_Type;

	class NODISCARD ISM_API CoreInt : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreInt;
		using storage_type = ssize_t;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreInt_Type }; }

	public:
		virtual ~CoreInt() override;
		CoreInt(self_type const & o) : base_type{ o }, m_data{ o.m_data } {}
		CoreInt() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;
		
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> self_type & operator=(T v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// float
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreFloat_Type;

	class NODISCARD ISM_API CoreFloat : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreFloat;
		using storage_type = double_t;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreFloat_Type }; }

	public:
		virtual ~CoreFloat() override;
		CoreFloat(self_type const & o) : base_type{ o }, m_data{ o.m_data } {}
		CoreFloat() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;
		
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> self_type & operator=(T v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// string
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreString_Type;

	class NODISCARD ISM_API CoreString : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreString;
		using storage_type = String;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreString_Type }; }

	public:
		virtual ~CoreString() override;
		CoreString(self_type const & o) : base_type{ o }, m_data{ o.m_data } {}
		CoreString(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreString(cstring v, size_t n) : base_type{ type_static() }, m_data{ v, n } {}
		CoreString(cstring v) : base_type{ type_static() }, m_data{ v } {}
		CoreString() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;

		template <class O
		> CoreString(ObjectAPI<O> const & o) : self_type{}
		{
			if (ISM::isinstance<STR>(o))
			{
				auto s{ const_cast<CoreString *>(static_cast<CoreString const *>(o.handle().ptr())) };
				VERIFY(s);
				m_data = s->m_data;
			}
			else
			{
				m_data = "?";
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto c_str() const noexcept { return m_data.c_str(); }
		NODISCARD auto data() const noexcept { return m_data.data(); }
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		NODISCARD auto begin() noexcept -> storage_type::iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> storage_type::const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> storage_type::const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> storage_type::iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> storage_type::const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> storage_type::const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// list
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreList_Type;

	class NODISCARD ISM_API CoreList : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreList;
		using storage_type = Vector<OBJECT>;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreList_Type }; }

	public:
		virtual ~CoreList() override;
		CoreList(self_type const & o) : base_type{ o }, m_data{} {}
		CoreList(std::initializer_list<OBJECT> v) : base_type{ type_static() }, m_data{ v } {}
		CoreList(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreList() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;

		template <class O
		> CoreList(ObjectAPI<O> const & o) : self_type{}
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto data() const noexcept { return m_data.data(); }
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		NODISCARD auto begin() noexcept -> storage_type::iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> storage_type::const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> storage_type::const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> storage_type::iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> storage_type::const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> storage_type::const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// dict
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreDict_Type;

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreDict;
		using storage_type = HashMap<OBJECT, OBJECT>;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreDict_Type }; }

	public:
		virtual ~CoreDict() override;
		CoreDict(self_type const & o) : base_type{ o }, m_data{ o.m_data } {}
		CoreDict(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreDict() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;

		template <class O
		> CoreDict(ObjectAPI<O> const & o) : self_type{}
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		NODISCARD auto begin() noexcept -> storage_type::iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> storage_type::const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> storage_type::const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> storage_type::iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> storage_type::const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> storage_type::const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// capsule
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreCapsule_Type;

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreCapsule;
		struct storage_type
		{
			void * ptr{};
			String name{};
		}
		m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreCapsule_Type }; }

	public:
		virtual ~CoreCapsule() override;
		CoreCapsule(self_type const & o) : base_type{ o }, m_data{} {}
		CoreCapsule(void const * ptr, String const & name = "") : base_type{ type_static() }, m_data{ (void *)ptr, name } {}
		CoreCapsule(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreCapsule() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;

		template <class O
		> CoreCapsule(ObjectAPI<O> const & o) : self_type{}
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// cast
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct type_caster_generic
	{
	};

	template <class T> struct type_caster_base : type_caster_generic
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace IMPL { template <class T, class SFINAE = void> struct type_caster : type_caster_base<T> {}; }

	template <class T> ALIAS(make_caster) IMPL::type_caster<intrinsic_t<T>>;

	namespace IMPL
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct type_caster<bool>
		{
			NODISCARD static OBJECT cast(bool src, return_value_policy, OBJECT)
			{
				return (OBJECT)Core_Boolean(src);
			}

			static bool convert(void const * src)
			{
				return src != nullptr;
			}

			static void encode(bool src, void *& dst)
			{
				dst = (void *)(src ? 1 : 0);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<T, std::enable_if_t<std::is_integral_v<T> && !is_char_v<T>>>
		{
			NODISCARD static OBJECT cast(T src, return_value_policy, OBJECT)
			{
				return INT{}.instance(src);
			}

			static auto convert(void const * src) -> T
			{
				if constexpr (sizeof(void *) == sizeof(int32_t))
				{
					return static_cast<T>(ISM::bit_cast<int32_t>(src));
				}
				else
				{
					return static_cast<T>(ISM::bit_cast<int64_t>(src));
				}
			}

			static void encode(T src, void *& dst)
			{
				if constexpr (sizeof(void *) == sizeof(int32_t))
				{
					dst = ISM::bit_cast<void *>(static_cast<int32_t>(src));
				}
				else
				{
					dst = ISM::bit_cast<void *>(static_cast<int64_t>(src));
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<T, std::enable_if_t<std::is_floating_point_v<T>>>
		{
			NODISCARD static OBJECT cast(T src, return_value_policy, OBJECT)
			{
				return FLT{}.instance(src);
			}

			static auto convert(void const * src) -> T
			{
				if constexpr (sizeof(void *) == sizeof(float32_t))
				{
					return static_cast<T>(ISM::bit_cast<float32_t>(src));
				}
				else
				{
					return static_cast<T>(ISM::bit_cast<float64_t>(src));
				}
			}

			static void encode(T src, void *& dst)
			{
				if constexpr (sizeof(void *) == sizeof(float32_t))
				{
					dst = ISM::bit_cast<void *>(static_cast<float32_t>(src));
				}
				else
				{
					dst = ISM::bit_cast<void *>(static_cast<float64_t>(src));
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct void_caster
		{
			NODISCARD static OBJECT cast(T, return_value_policy, OBJECT)
			{
				return nullptr;
			}
		};

		template <> struct type_caster<void_type> : void_caster<void_type> {};

		template <> struct type_caster<void> : type_caster<void_type>
		{
			using type_caster<void_type>::cast;

			NODISCARD static OBJECT cast(void const * src, return_value_policy, OBJECT)
			{
				CAPSULE out{};
				if (src) { out.instance(src); }
				return out;
			}
		};

		template <> struct type_caster<nullptr_t> : void_caster<nullptr_t> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S> struct string_caster
		{
			using string_type = typename S;

			NODISCARD static OBJECT cast(string_type const & src, return_value_policy, OBJECT)
			{
				return STR{}.instance(src);
			}

			static string_type convert(void const * src)
			{
				return *CHECK((string_type const *)src);
			}

			static void encode(string_type const & src, void *& dst)
			{
				dst = (void *)std::addressof(src);
			}
		};

		template <class Ch, class Tr, class Al
		> struct type_caster<std::basic_string<Ch, Tr, Al>, std::enable_if_t<is_char_v<Ch>>>
			: string_caster<std::basic_string<Ch, Tr, Al>> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct type_caster<T, std::enable_if_t<is_char_v<T>>> : type_caster<std::basic_string<T>>
		{
			using type_caster<std::basic_string<T>>::cast;

			NODISCARD static auto cast(T const src, return_value_policy, OBJECT)
			{
				return INT{}.instance(src);
			}

			NODISCARD static auto cast(T const * src, return_value_policy, OBJECT)
			{
				return STR{}.instance(src);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct core_caster
		{
			NODISCARD static OBJECT cast(OBJECT const & src, return_value_policy, OBJECT)
			{
				return src;
			}
		};

		template <class T> struct type_caster<T, std::enable_if_t<is_core_v<T>>> : core_caster<T> {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// core -> core
	template <class T, class O, std::enable_if_t<is_core_v<T> && is_core_v<O>, int> = 0
	> NODISCARD decltype(auto) cast(O && o) { return T{ FWD(o) }; }

	// core -> c++
	template <class T, class O, std::enable_if_t<!is_core_v<T> && is_core_v<O>, int> = 0
	> NODISCARD decltype(auto) cast(O && o)
	{
		if constexpr (std::is_same_v<O, ISM::make_handle<T>>)
		{
			return static_cast<T>(***FWD(o));
		}
		else
		{
			return ISM::cast<T>(ISM::cast<make_handle<T>>(FWD(o)));
		}
	}

	// c++ -> core
	template <class T, std::enable_if_t<!is_core_v<T>, int> = 0
	> NODISCARD OBJECT cast(T && o,
		return_value_policy policy = return_value_policy::automatic_reference,
		OBJECT parent = nullptr)
	{
		return ISM::make_caster<T>::cast(FWD(o), policy, parent);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O> template <class T> inline T Handle<O>::cast() const &
	{
		if constexpr (!std::is_same_v<T, void>) { return ISM::cast<T>(*this); }
	}

	template <class O> template <class T> inline T Handle<O>::cast() &&
	{
		if constexpr (!std::is_same_v<T, void>) { return ISM::cast<T>(std::move(*this)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<!is_core_v<T>, int>
	> NODISCARD OBJECT object_or_cast(T && o) { return ISM::cast(FWD(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> auto object_convert(void const * src) noexcept -> decltype(auto)
	{
		return ISM::make_caster<T>::convert(src);
	}

	template <class T, class U = T
	> void object_encode(U && src, void *& dst) noexcept
	{
		ISM::make_caster<T>::encode(FWD(src), dst);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline OBJECT getattr(OBJECT const & o, OBJECT const & i)
	{
		TYPE t{ o.type() };
		VERIFY(t);
		if (!ISM::isinstance<STR>(i)) { return nullptr; }
		else if (t->tp_getattro) { return t->tp_getattro(o, i); }
		else if (t->tp_getattr) { return t->tp_getattr(o, STR(i)->c_str()); }
		else return nullptr;
	}

	NODISCARD inline OBJECT getattr(OBJECT const & o, cstring i)
	{
		TYPE t{ o.type() };
		VERIFY(t);
		if (t->tp_getattro) { return t->tp_getattro(o, ISM::object_or_cast(i)); }
		else if (t->tp_getattr) { return t->tp_getattr(o, i); }
		else return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class V = OBJECT
	> Err setattr(OBJECT o, OBJECT const & i, V && v)
	{
		TYPE t{ o.type() };
		VERIFY(t);
		if (!ISM::isinstance<STR>(i)) { return Err_Unknown; }
		else if (t->tp_getattro) { return t->tp_setattro(o, i, ISM::object_or_cast(FWD(v))); }
		else if (t->tp_setattr) { return t->tp_setattr(o, STR(i)->c_str(), ISM::object_or_cast(FWD(v))); }
		else return Err_Unknown;
	}

	template <class V = OBJECT
	> Err setattr(OBJECT o, cstring i, V && v)
	{
		TYPE t{ o.type() };
		VERIFY(t);
		if (t->tp_getattro) { return t->tp_setattro(o, ISM::object_or_cast(i), ISM::object_or_cast(FWD(v))); }
		else if (t->tp_setattr) { return t->tp_setattr(o, i, ISM::object_or_cast(FWD(v))); }
		else return Err_Unknown;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class I = OBJECT
	> NODISCARD OBJECT getitem(OBJECT o, I && i)
	{
		if (ISM::isinstance<DICT>(o))
		{
			return (***DICT(o))[ISM::object_or_cast(FWD(i))];
		}
		else
		{
			return nullptr;
		}
	}

	template <class I = OBJECT, class V = OBJECT
	> Err setitem(OBJECT o, I && i, V && v)
	{
		if (ISM::isinstance<DICT>(o))
		{
			return ((***DICT(o))[ISM::object_or_cast(FWD(i))] = ISM::object_or_cast(FWD(v))), Err_None;
		}
		else
		{
			return Err_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<class Derived
	> template <class ...Args
	> inline OBJECT ObjectAPI<Derived>::operator()(Args && ... args)
	{
		return Core_None;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// method
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreMethod_Type;

	class NODISCARD ISM_API CoreMethod : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreMethod;
		using storage_type = typename CALLABLE;
		storage_type m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreMethod_Type }; }

	public:
		virtual ~CoreMethod() override;
		CoreMethod() noexcept : base_type{ type_static() }, m_data{} {}
		CoreMethod(nullptr_t) noexcept : base_type{ type_static() }, m_data{ nullptr } {}
		CoreMethod(self_type const & o) : base_type{ o }, m_data{ o.m_data } {}
		CoreMethod(self_type && o) noexcept : base_type{ o }, m_data{ std::move(o.m_data) } {}
		CoreMethod(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreMethod(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		template <class O
		> CoreMethod(ObjectAPI<O> const & o) : self_type{}
		{
		}

		template <class Arg0, class ... Args
		> CoreMethod(Arg0 && arg0, Args && ... args) : self_type{}
		{
			m_data = ISM::bind_method(FWD(arg0), FWD(args)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// generic
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreGeneric_Type;

	NODISCARD inline OBJECT make_generic(TYPE const & t) { return t->tp_create(); }

	template <class T, std::enable_if_t<is_core_v<T>, int> = 0
	> NODISCARD OBJECT make_generic() { return ISM::typeof<T>()->tp_create(); }

	class ISM_API CoreGeneric : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreGeneric;
		using storage_type = void;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreGeneric_Type }; }

	public:
		virtual ~CoreGeneric();
		explicit CoreGeneric(TYPE const & t) : base_type{ t } {}
		CoreGeneric() : self_type{ type_static() } {}
		CoreGeneric(self_type const &) = default;
		CoreGeneric(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// module
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType) _CoreModule_Type;

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
	public:
		using base_type = typename CoreObject;
		using self_type = typename CoreModule;
		struct storage_type
		{
			String name{};
			String doc{};
			OBJECT dict{};
		}
		m_data;

		NODISCARD static auto type_static() noexcept { return TYPE{ &_CoreModule_Type }; }

	public:
		virtual ~CoreModule() override;
		CoreModule(self_type const & o) : base_type{ o }, m_data{} {}
		CoreModule(String const & name, String const & doc = "") : base_type{ type_static() }, m_data{ name, doc } {}
		CoreModule(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreModule() : base_type{ type_static() }, m_data{} {}
		self_type & operator=(self_type const &) = default;

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// misc
namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static OBJECT CoreNumber_Op_Invert(OBJECT obj) { return nullptr; }
	static OBJECT CoreNumber_Op_Negative(OBJECT obj) { return nullptr; }
	static OBJECT CoreNumber_Op_Add(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Subtract(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Multiply(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Divide(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Or(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_And(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Xor(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Lshift(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_Rshift(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Add(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Subtract(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Multiply(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Divide(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Or(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_And(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Xor(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Lshift(OBJECT lhs, OBJECT rhs) { return nullptr; }
	static OBJECT CoreNumber_Op_InPlace_Rshift(OBJECT lhs, OBJECT rhs) { return nullptr; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RUNTIME_HPP_
