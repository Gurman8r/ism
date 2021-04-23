#ifndef _ISM_TYPES_HPP_
#define _ISM_TYPES_HPP_

#include <core/api/detail/common.hpp>

// data
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)		_CoreObject_Type;
#define CoreObject_Type			(ism::TYPE(&ism::_CoreObject_Type))

	ISM_API_DATA(CoreType)		_CoreType_Type;
#define CoreType_Type			(ism::TYPE(&ism::_CoreType_Type))

	ISM_API_DATA(CoreType)		_CoreNone_Type;
#define CoreNone_Type			(ism::TYPE(&ism::_CoreNone_Type))

	ISM_API_DATA(CoreType)		_CoreBool_Type;
#define CoreBool_Type			(ism::TYPE(&ism::_CoreBool_Type))

	ISM_API_DATA(CoreType)		_CoreInt_Type;
#define CoreInt_Type			(ism::TYPE(&ism::_CoreInt_Type))

	ISM_API_DATA(CoreType)		_CoreFloat_Type;
#define CoreFloat_Type			(ism::TYPE(&ism::_CoreFloat_Type))

	ISM_API_DATA(CoreType)		_CoreString_Type;
#define CoreString_Type			(ism::TYPE(&ism::_CoreString_Type))

	ISM_API_DATA(CoreType)		_CoreList_Type;
#define CoreList_Type			(ism::TYPE(&ism::_CoreList_Type))

	ISM_API_DATA(CoreType)		_CoreDict_Type;
#define CoreDict_Type			(ism::TYPE(&ism::_CoreDict_Type))

	ISM_API_DATA(CoreType)		_CoreCapsule_Type;
#define CoreCapsule_Type		(ism::TYPE(&ism::_CoreCapsule_Type))

	ISM_API_DATA(CoreType)		_CoreFunction_Type;
#define CoreFunction_Type		(ism::TYPE(&ism::_CoreFunction_Type))

	ISM_API_DATA(CoreType)		_CoreProperty_Type;
#define CoreProperty_Type		(ism::TYPE(&ism::_CoreProperty_Type))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreObject)	_Core_None;
#define Core_None				(ism::OBJECT(&ism::_Core_None))

	ISM_API_DATA(CoreInt)		_Core_True;
	ISM_API_DATA(CoreInt)		_Core_False;
#define Core_True				(ism::INT(&ism::_Core_True))
#define Core_False				(ism::INT(&ism::_Core_False))
#define Core_Bool(b)			((b) ? Core_True : Core_False)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : _API_Tag
	{
	private:
		NODISCARD auto derived() const noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		NODISCARD auto handle() const & noexcept
		{
			if constexpr (is_core_object_v<Derived>)
			{
				return Handle<Derived>{ derived().ptr() };
			}
			else
			{
				return derived();
			}
		}

		template <class I = cstring
		> NODISCARD auto attr(I && i) const { return AttrAccessor<I>{ handle(), FWD(i) }; }

		template <class I = cstring
		> NODISCARD auto operator[](I && i) const { return ItemAccessor<I>{ handle(), FWD(i) }; }

		template <class V = OBJECT
		> NODISCARD bool contains(V && v) const { return attr("__contains__")(FWD(v)).cast<bool>(); }

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJECT operator()(Args && ... args); // call.hpp
		
		NODISCARD bool is_none() const noexcept { return derived().ptr() == &_Core_None; }
		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }

		NODISCARD bool is(ObjectAPI const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }
		NODISCARD bool equal_to(ObjectAPI const & o) const noexcept { return compare(o) == 0; }
		NODISCARD bool not_equal_to(ObjectAPI const & o) const noexcept { return compare(o) != 0; }
		NODISCARD bool less(ObjectAPI const & o) const noexcept { return compare(o) < 0; }
		NODISCARD bool less_equal(ObjectAPI const & o) const noexcept { return compare(o) <= 0; }
		NODISCARD bool greater(ObjectAPI const & o) const noexcept { return compare(o) > 0; }
		NODISCARD bool greater_equal(ObjectAPI const & o) const noexcept { return compare(o) >= 0; }

		NODISCARD auto compare(ObjectAPI const & o) const -> int32_t
		{
			if (TYPE t{ typeof(derived().ptr()) }; t && t->tp_compare)
			{
				return t->tp_compare(handle(), o.handle());
			}
			else
			{
				return util::compare((void *)derived().ptr(), (void *)o.derived().ptr());
			}
		}

		NODISCARD auto doc() const { return attr("__doc__"); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD bool operator==(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.equal_to(b); }

	template <class T
	> NODISCARD bool operator!=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.not_equal_to(b); }

	template <class T
	> NODISCARD bool operator<(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less(b); }

	template <class T
	> NODISCARD bool operator<=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less_equal(b); }

	template <class T
	> NODISCARD bool operator>(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater(b); }

	template <class T
	> NODISCARD bool operator>=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// handle
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Handle : public ObjectAPI<Handle<T>>, public Ref<T>
	{
	public:
		NODISCARD static auto type_static() { return typeof<T>(); }

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;

	public:
		~Handle()
		{
			if (m_ref && m_ref->dec_ref())
			{
				if (TYPE t{ typeof(m_ref) }; t && t->tp_delete)
				{
					t->tp_delete(m_ref);
				}
				else
				{
					ism::memdelete(m_ref);
				}
			}
			m_ref = nullptr;
		}

		Handle() {}
		
		Handle(nullptr_t) {}

		Handle(T * value) { if (value) { ref_pointer(value); } }
		
		Handle(Handle const & value) { ref(value); }
		
		template <class U
		> Handle(Handle<U> const & value) { reset(value); }

		Handle(T const & value) { revalue(value); }

		Handle(T && value) noexcept { revalue(std::move(value)); }

		Handle & operator=(nullptr_t) { unref(); return (*this); }

		Handle & operator=(Handle const & value) { reset(value); return (*this); }
		
		template <class U
		> Handle & operator=(Handle<U> const & value) { reset(value); return (*this); }
		
		Handle & operator=(T const & value) { revalue(value); return (*this); }

		Handle & operator=(T && value) noexcept { revalue(std::move(value)); return (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ism::Hash<Handle<T>>
	{
		NODISCARD hash_t operator()(Handle<T> const & o) const { return ism::hash(o); }
	};

	template <class T> struct ism::EqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct ism::NotEqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct ism::Less<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct ism::Greater<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct ism::LessEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct ism::GreaterEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// accessors
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy
	> class Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using key_type = typename Policy::key_type;

		Accessor(OBJECT obj, key_type && key) : m_obj{ obj }, m_key{ FWD(key) } {}
		Accessor(Accessor const &) = default;
		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT(a)); }
		void operator=(Accessor const & a) & { operator=(OBJECT(a)); }

		template <class T> decltype(auto) operator=(T && v) &&
		{
			Policy::set(m_obj, m_key, detail::object_or_cast(FWD(v)));
			return (*this);
		}

		template <class T> decltype(auto) operator=(T && v) &
		{
			get_cache() = detail::object_or_cast(FWD(v));
			return (*this);
		}

		NODISCARD auto ptr() const { return const_cast<CoreObject *>(get_cache().ptr()); }

		template <class T> NODISCARD operator Handle<T>() const { return get_cache(); }

		template <class T> NODISCARD auto cast() const -> T { return get_cache().cast<T>(); }

	protected:
		OBJECT & get_cache() const {
			if (!m_cache) { m_cache = Policy::get(m_obj, m_key); }
			return m_cache;
		}

	private:
		OBJECT m_obj;
		key_type m_key;
		mutable OBJECT m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct accessor_policies::Attr
	{
		using key_type = T;

		template <class O = OBJECT, class I = T
		> static OBJECT get(O && o, I && i) { return ism::getattr(FWD(o), FWD(i)); }

		template <class O = OBJECT, class I = T, class V = OBJECT
		> static void set(O && o, I && i, V && v) { ism::setattr(FWD(o), FWD(i), FWD(v)); }
	};

	template <class T> struct accessor_policies::Item
	{
		using key_type = T;

		template <class O = OBJECT, class I = T
		> static OBJECT get(O && o, I && i) { return ism::getitem(FWD(o), FWD(i)); }

		template <class O = OBJECT, class I = T, class V = OBJECT
		> static void set(O && o, I && i, V && v) { ism::setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreObject : public ObjectAPI<CoreObject>, public Reference
	{
	protected:
		using base_type = void;
		using self_type = CoreObject;

	public:
		NODISCARD static TYPE type_static() { return CoreObject_Type; }
		explicit CoreObject(TYPE const & t) : Reference{}, ob_type{ t } {}
		CoreObject() : self_type{ type_static() } {}
		CoreObject(CoreObject const &) = default;
		CoreObject(CoreObject &&) noexcept = default;
		CoreObject & operator=(CoreObject const &) = default;
		CoreObject & operator=(CoreObject &&) noexcept = default;

	public:
		TYPE ob_type{};

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;

		NODISCARD auto ptr() const { return const_cast<CoreObject *>(this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_CLASS_COMMON(m_class, m_inherits, m_type_static)	\
protected:														\
	using self_type = m_class;									\
	using base_type = m_inherits;								\
public:															\
	NODISCARD static TYPE type_static()							\
	{															\
		return m_type_static;									\
	}															\
	NODISCARD static constexpr StringView name_static()			\
	{															\
		return TOSTR(m_class);									\
	}															\
	explicit m_class(TYPE const & type) : m_inherits{ type }	\
	{															\
	}															\
private:


#define ISM_CLASS_CVT(m_class, m_inherits, m_type_static)		\
	ISM_CLASS_COMMON(m_class, m_inherits, m_type_static);		\
public:															\
	m_class(m_class const &) = default;							\
	m_class(m_class &&) noexcept = default;						\
	m_class & operator=(m_class const &) = default;				\
	m_class & operator=(m_class &&) noexcept = default;			\
private:


#define ISM_CLASS_DEFAULT(m_class, m_inherits, m_type_static)	\
	ISM_CLASS_CVT(m_class, m_inherits, m_type_static);			\
public:															\
	m_class() : m_inherits{ m_class::type_static() } {}			\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
		> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

		template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
		> NODISCARD OBJECT object_or_cast(T && o);

		NODISCARD inline OBJECT object_or_cast(CoreObject * o) { return OBJECT{ o }; }

		NODISCARD inline OBJECT object_or_cast(cstring s) { return object_or_cast(String{ s }); }

		NODISCARD inline TYPE typeof_generic(std::type_info const & t);

		NODISCARD inline bool isinstance_generic(OBJECT const & o, std::type_info const & t);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return T::type_static();
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return detail::typeof_generic(typeid(T));
	}

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o)
	{
		return o ? o->ob_type : nullptr;
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return typeof(detail::object_or_cast(FWD(o)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return typeof(o).is(typeof<T>());
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return detail::isinstance_generic(o, typeid(T));
	}

	template <class A, class B = A
	> NODISCARD bool isinstance(A const & a, B const & b)
	{
		return typeof(a).is(isinstance<TYPE>(b) ? b : typeof(b));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD auto hash(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_hash ? t->tp_hash(o) : hash_t{ 0 };
	}

	template <class T> NODISCARD auto len(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_len ? t->tp_len(o) : ssize_t{ -1 };
	}

	template <class T> NODISCARD auto repr(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_repr ? t->tp_repr(o) : STR{ nullptr };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class I = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, I && i)
	{
		if (TYPE t{ typeof(o) }; !t)
		{
			return nullptr;
		}
		else if constexpr (is_string_v<I>)
		{
			if (t->tp_getattr)
			{
				return t->tp_getattr(o, FWD(i));
			}
			else if (t->tp_getattro)
			{
				return t->tp_getattro(o, detail::object_or_cast(FWD(i)));
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			if (t->tp_getattro)
			{
				return t->tp_getattro(o, detail::object_or_cast(FWD(i)));
			}
			else if (t->tp_getattr)
			{
				return t->tp_getattr(o, STR(FWD(i))->c_str());
			}
			else
			{
				return nullptr;
			}
		}
	}

	template <class I = OBJECT, class D = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, I && i, D && defval) noexcept
	{
		OBJECT result{ getattr(o, FWD(i)) };
		return result ? result : detail::object_or_cast(FWD(defval));
	}

	template <class I = OBJECT
	> NODISCARD bool hasattr(OBJECT o, I && i)
	{
		return getattr(FWD(o), FWD(i)).is_valid();
	}

	template <class I = OBJECT, class V = OBJECT
	> Error setattr(OBJECT o, I && i, V && v)
	{
		if (TYPE t{ typeof(o) }; !t)
		{
			return Error_Unknown;
		}
		else if constexpr (is_string_v<I>)
		{
			if (t->tp_setattr)
			{
				return t->tp_setattr(o, FWD(i), detail::object_or_cast(FWD(v)));
			}
			else if (t->tp_getattro)
			{
				return t->tp_setattro(o, detail::object_or_cast(i), detail::object_or_cast(FWD(v)));
			}
			else
			{
				return Error_Unknown;
			}
		}
		else
		{
			if (t->tp_getattro)
			{
				return t->tp_setattro(o, detail::object_or_cast(FWD(i)), detail::object_or_cast(FWD(v)));
			}
			else if (t->tp_setattr)
			{
				return t->tp_setattr(o, STR(FWD(i))->c_str(), detail::object_or_cast(FWD(v)));
			}
			else
			{
				return Error_Unknown;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class I = OBJECT
	> NODISCARD OBJECT getitem(OBJECT o, I && i)
	{
		if (!o.is_valid())
		{
			return nullptr;
		}
		else if (isinstance<DICT>(o))
		{
			return (***DICT(o))[detail::object_or_cast(FWD(i))];
		}
		else if (isinstance<LIST>(o))
		{
			return (***LIST(o))[detail::object_or_cast(FWD(i)).cast<size_t>()];
		}
		else if (OBJECT * dict{ detail::get_dict_ptr(o) })
		{
			return getitem(*dict, FWD(i));
		}
		else
		{
			return nullptr;
		}
	}

	template <class I = OBJECT, class V = OBJECT
	> Error setitem(OBJECT o, I && i, V && v)
	{
		if (!o.is_valid())
		{
			return Error_Unknown;
		}
		else if (isinstance<DICT>(o))
		{
			return ((***DICT(o))[detail::object_or_cast(FWD(i))] = detail::object_or_cast(FWD(v))), Error_None;
		}
		else if (isinstance<LIST>(o))
		{
			return ((***LIST(o))[detail::object_or_cast(FWD(i)).cast<size_t>()] = detail::object_or_cast(FWD(v))), Error_None;
		}
		else if (OBJECT * dict{ detail::get_dict_ptr(o) })
		{
			return setitem(*dict, FWD(i), FWD(v));
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// type
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreType : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreType, CoreObject, CoreType_Type);

	public:
		String				tp_name{};
		ssize_t				tp_basicsize{}, tp_itemsize{};
		int32_t				tp_flags{};
		String				tp_doc{};

		ssize_t				tp_dict_offset{};
		ssize_t				tp_weaklist_offset{};
		ssize_t				tp_vectorcall_offset{};

		binaryfunc			tp_call{};
		inquiry				tp_clear{};
		cmpfunc				tp_compare{};
		hashfunc			tp_hash{};
		lenfunc				tp_len{};
		reprfunc			tp_repr{};
		reprfunc			tp_str{};
		traverseproc		tp_traverse{};

		getattrfunc			tp_getattr{};
		setattrfunc			tp_setattr{};
		getattrofunc		tp_getattro{};
		setattrofunc		tp_setattro{};
		descrgetfunc		tp_descrget{};
		descrsetfunc		tp_descrset{};

		TYPE				tp_base{};
		OBJECT				tp_bases{};
		OBJECT				tp_cache{};
		OBJECT				tp_mro{};
		OBJECT				tp_subclasses{};

		destructor			tp_delete{};
		initproc			tp_init{};
		allocfunc			tp_alloc{};
		newfunc				tp_new{};
		freefunc			tp_free{};
		inquiry				tp_is_gc{};
		destructor			tp_finalize{};

		GetSetDef *			tp_getsets{};
		MethodDef *			tp_methods{};

		NumberMethods *		tp_as_number{};
		SequenceMethods *	tp_as_sequence{};
		MappingMethods *	tp_as_mapping{};

		OBJECT				tp_dict{};
		OBJECT				tp_weaklist{};
		vectorcallfunc		tp_vectorcall{};

	public:
		NODISCARD bool has_feature(int32_t feature) const noexcept { return flag_read(tp_flags, feature); }

		void enable_feature(int32_t feature) noexcept { flag_set(tp_flags, feature); }

		void disable_feature(int32_t feature) noexcept { flag_clear(tp_flags, feature); }

		NODISCARD bool ready();

	protected:
		void add_methods(MethodDef * methods);
		void add_getsets(GetSetDef * getsets);
		bool add_subclass(TYPE const & type);
		bool mro_internal(OBJECT * old_mro);
		void inherit_special(CoreType const * base);
		void inherit_slots(CoreType const * base);

	protected:
		template <class Slot> bool slot_defined(CoreType const * base, Slot CoreType::*slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot NumberMethods::*slot) const
		{
			return (this->tp_as_number->*slot) && (!base || (this->tp_as_number->*slot) != (base->tp_as_number->*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot SequenceMethods::*slot) const
		{
			return (this->tp_as_sequence->*slot) && (!base || (this->tp_as_sequence->*slot) != (base->tp_as_sequence->*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot MappingMethods::*slot) const
		{
			return (this->tp_as_mapping->*slot) && (!base || (this->tp_as_mapping->*slot) != (base->tp_as_mapping->*slot));
		}

	protected:
		template <class Slot> void copy_slot(CoreType const * base, Slot CoreType::*slot)
		{
			if (!(this->*slot) && base)
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot CoreType::*slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot NumberMethods::*slot)
		{
			if (!(this->tp_as_number->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->tp_as_number->*slot) = (base->tp_as_number->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot SequenceMethods::*slot)
		{
			if (!(this->tp_as_sequence->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->tp_as_sequence->*slot) = (base->tp_as_sequence->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot MappingMethods::*slot)
		{
			if (!(this->tp_as_mapping->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->tp_as_mapping->*slot) = (base->tp_as_mapping->*slot);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		NODISCARD inline GetSetDef * get_getset_def(OBJECT o, cstring name)
		{
			if (hash_t id{ hash(name, strlen(name)) }; TYPE type{ typeof(o) })
			{
				for (GetSetDef * def{ type->tp_getsets }; def && *def; ++def)
				{
					if (id == hash(def->name, strlen(def->name)))
					{
						return def;
					}
				}
			}
			return nullptr;
		}

		NODISCARD inline MethodDef * get_method_def(OBJECT o, cstring name)
		{
			if (hash_t id{ hash(name, strlen(name)) }; TYPE type{ typeof(o) })
			{
				for (MethodDef * def{ type->tp_methods }; def && *def; ++def)
				{
					if (id == hash(def->name, strlen(def->name)))
					{
						return def;
					}
				}
			}
			return nullptr;
		}

		NODISCARD inline auto get_as_number(OBJECT const & o)
		{
			TYPE t{ typeof(o) };

			return t ? t->tp_as_number : (NumberMethods *)nullptr;
		}

		NODISCARD inline auto get_as_sequence(OBJECT const & o)
		{
			TYPE t{ typeof(o) };

			return t ? t->tp_as_sequence : (SequenceMethods *)nullptr;
		}

		NODISCARD inline auto get_as_mapping(OBJECT const & o)
		{
			TYPE t{ typeof(o) };

			return t ? t->tp_as_mapping : (MappingMethods *)nullptr;
		}

		NODISCARD inline auto get_dict_ptr(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && 0 < t->tp_dict_offset)
			{
				return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(o.ptr()) + t->tp_dict_offset);
			}
			else
			{
				return (OBJECT *)nullptr;
			}
		}

		NODISCARD inline auto get_weaklist_ptr(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && 0 < t->tp_weaklist_offset)
			{
				return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(o.ptr()) + t->tp_weaklist_offset);
			}
			else
			{
				return (OBJECT *)nullptr;
			}
		}

		NODISCARD inline auto get_vectorcall_func(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && t->has_feature(TypeFlags_HaveVectorCall) && 0 < t->tp_vectorcall_offset)
			{
				return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(o.ptr()) + t->tp_vectorcall_offset);
			}
			else
			{
				return (vectorcallfunc)nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// int
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreInt : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreInt, CoreObject, CoreInt_Type);

		int64_t m_data{};

		using storage_type = decltype(m_data);

	public:
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt & operator=(T const v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// float
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreFloat : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreFloat, CoreObject, CoreFloat_Type);

		double_t m_data{};

		using storage_type = decltype(m_data);

	public:
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat & operator=(T const v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreString : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreString, CoreObject, CoreString_Type);

		String m_data{};

		using storage_type = decltype(m_data);

	public:
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreString(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}

		CoreString(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}

		CoreString(cstring v) : base_type{ type_static() }, m_data{ v } {}

		CoreString(cstring v, size_t n) : base_type{ type_static() }, m_data{ v, n } {}

		CoreString(std::initializer_list<item_type> init) : self_type{ storage_type{ init.begin(), init.end() } } {}

		template <class T> CoreString(Handle<T> const & o) noexcept : self_type{}
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				m_data = ***o;
			}
			else if (isinstance<STR>(o))
			{
				m_data = ***STR{ o };
			}
			else if (TYPE t{ typeof(o) }; t && t->tp_str)
			{
				m_data = ***t->tp_str(o);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto c_str() const noexcept { return m_data.c_str(); }
		
		NODISCARD auto data() const noexcept { return m_data.data(); }
		
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		
		NODISCARD auto size() const noexcept { return m_data.size(); }
		
		NODISCARD auto front() const { return m_data.front(); }
		
		NODISCARD auto back() const { return m_data.back(); }
		
		void reserve(size_t count) { m_data.reserve(count); }
		
		void resize(size_t count) { m_data.reserve(count); }
		
		void shrink_to_fit() { m_data.shrink_to_fit(); }

		void erase(size_t i) { m_data.erase(begin() + i); }

		void erase(OBJECT const & i) { m_data.erase(begin() + i.cast<size_t>()); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// list
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreList : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreList, CoreObject, CoreList_Type);

		Vector<OBJECT> m_data{};

		using storage_type = decltype(m_data);

	public:
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreList(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}

		CoreList(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}

		CoreList(std::initializer_list<item_type> init) : self_type{}
		{
			m_data.reserve(init.size());
			for (item_type const & e : init) {
				m_data.push_back(e);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto data() const noexcept { return m_data.data(); }
		
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		
		NODISCARD auto size() const noexcept { return m_data.size(); }
		
		NODISCARD auto front() const { return m_data.front(); }
		
		NODISCARD auto back() const { return m_data.back(); }
		
		void reserve(size_t count) { m_data.reserve(count); }
		
		void resize(size_t count) { m_data.reserve(count); }
		
		void shrink_to_fit() { m_data.shrink_to_fit(); }

		void erase(size_t i) { m_data.erase(begin() + i); }

		void erase(OBJECT const & i) { m_data.erase(begin() + i.cast<size_t>()); }

		template <class V = OBJECT
		> bool contains(V && v) const { return end() != std::find(begin(), end(), detail::object_or_cast(FWD(v))); }

		template <class V = OBJECT
		> void append(V && v) { m_data.emplace_back(detail::object_or_cast(FWD(v))); }

		template <class V = OBJECT
		> void insert(size_t i, V && v) { m_data.insert(begin() + i, detail::object_or_cast(FWD(v))); }

		template <class V = OBJECT
		> void insert(OBJECT const & i, V && v) { m_data.insert(begin() + i.cast<size_t>(), detail::object_or_cast(FWD(v))); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// dict
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreDict, CoreObject, CoreDict_Type);

		HashMap<OBJECT, OBJECT> m_data{};

		using storage_type = decltype(m_data);

	public:
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreDict(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}

		CoreDict(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}

		CoreDict(std::initializer_list<item_type> init) : self_type{}
		{
			m_data.reserve(init.size());
			for (item_type const & e : init) {
				m_data.insert(e);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		
		NODISCARD auto size() const noexcept { return m_data.size(); }

		template <class I = OBJECT
		> void erase(I && i) { m_data.erase(detail::object_or_cast(FWD(i))); }

		template <class I = OBJECT
		> bool contains(I && i) const { return end() != m_data.find(detail::object_or_cast(FWD(i))); }

		template <class I = OBJECT, class V = OBJECT
		> void insert(I && i, V && v) { m_data.try_emplace(detail::object_or_cast(FWD(i)), detail::object_or_cast(FWD(v))); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// capsule
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreCapsule, CoreObject, CoreCapsule_Type);

		struct storage_type
		{
			void * pointer{}, * context{};

			destructor closure{};

			String name{}, doc{};

			~storage_type() { if (closure) { closure((CoreObject *)pointer); } }
		}
		m_data{};

	public:
		CoreCapsule(nullptr_t) : self_type{}
		{
			m_data = { nullptr, nullptr, nullptr };
		}

		CoreCapsule(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_data = { (void *)value, nullptr, closure };
		}

		CoreCapsule(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_data = { (void *)value, (void *)closure, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->get_context()) };

					closure(self->get_pointer());
				}
			} };
		}

		CoreCapsule(void(*closure)()) : self_type{}
		{
			m_data = { (void *)closure, nullptr, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->get_pointer()) };

					closure();
				}
			} };
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto get_closure() const -> destructor { return m_data.closure; }
		
		NODISCARD auto get_context() const -> void * { return m_data.context; }
		
		NODISCARD auto get_pointer() const -> void * { return m_data.pointer; }

		void set_closure(destructor value) { m_data.closure = value; }
		
		void set_context(void const * value) { m_data.context = (void *)value; }
		
		void set_pointer(void const * value) { m_data.pointer = (void *)value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreFunction : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreFunction, CoreObject, CoreFunction_Type);

	public:
		DICT m_dict{ CoreDict{} };
		vectorcallfunc m_vectorcall{};

		CoreFunction(TYPE const & t, vectorcallfunc vectorcall) : base_type{ t }, m_vectorcall{ vectorcall } {}

		CoreFunction(vectorcallfunc vectorcall) : base_type{ type_static() }, m_vectorcall{ vectorcall } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// property
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct property_record
		{
			OBJECT get{};
			OBJECT set{};
			void * closure{};
			String name{}, doc{};
		};
	}

	class ISM_API CoreProperty : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreProperty, CoreObject, CoreProperty_Type);

		detail::property_record m_data{};

		using storage_type = decltype(m_data);

	public:
		CoreProperty(OBJECT fget, OBJECT fset, void * closure, String const & doc = "") : base_type{ type_static() }
		{
			m_data = { fget, fset, closure, "", doc };
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPES_HPP_
