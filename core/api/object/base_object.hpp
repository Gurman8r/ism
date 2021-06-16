#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/detail/common.hpp>

#define FWD_OBJ(expr) (ism::api::object_or_cast(FWD(expr)))

#define ISM_BUILTIN_TYPE(m_class, m_var) \
	DECLEXPR(m_class::ob_type_static) = COMPOSE(ism::api::TypeObject, m_var)

// object_api
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : _API_Tag
	{
	private:
		NODISCARD auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		NODISCARD auto handle() const & noexcept
		{
			if constexpr (is_base_object_v<Derived>)
			{
				return Handle<Derived>{ derived().ptr() };
			}
			else
			{
				return derived();
			}
		}

		template <class Index = cstring
		> NODISCARD auto attr(Index && i) const { return AttrAccessor<Index>{ handle(), FWD(i) }; }

		template <class Index = cstring
		> NODISCARD auto operator[](Index && i) const { return ItemAccessor<Index>{ handle(), FWD(i) }; }

		template <class Value = OBJECT
		> NODISCARD bool contains(Value && v) const { return attr("__contains__")(handle(), FWD(v)); }

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJECT operator()(Args && ... args) const; // call.hpp

		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }
		NODISCARD bool is(ObjectAPI const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }

		NODISCARD bool equal_to(ObjectAPI const & o) const noexcept { return compare(o) == 0; }
		NODISCARD bool not_equal_to(ObjectAPI const & o) const noexcept { return compare(o) != 0; }
		NODISCARD bool less(ObjectAPI const & o) const noexcept { return compare(o) < 0; }
		NODISCARD bool less_equal(ObjectAPI const & o) const noexcept { return compare(o) <= 0; }
		NODISCARD bool greater(ObjectAPI const & o) const noexcept { return compare(o) > 0; }
		NODISCARD bool greater_equal(ObjectAPI const & o) const noexcept { return compare(o) >= 0; }

		NODISCARD auto doc() const { return attr("__doc__"); }

	private:
		NODISCARD auto compare(ObjectAPI const & o) const
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
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base handle
	template <class T> class BaseHandle : public Ref<T>, public ObjectAPI<Handle<T>>
	{
	protected:
		BaseHandle() = default;

	public:
		~BaseHandle() = default;

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// handle default
#define ISM_HANDLE_DEFAULT(m_class)																	\
public:																								\
	Handle(nullptr_t) {}																			\
																									\
	Handle(m_class * value) { if (value) { ref_pointer(value); } }									\
																									\
	Handle(Ref<m_class> const & value) { ref(value); }												\
																									\
	template <class U> Handle(Ref<U> const & value) { reset(value); }								\
																									\
	Handle(m_class const & value) { instance(value); }												\
																									\
	Handle(m_class && value) noexcept { instance(std::move(value)); }								\
																									\
	Handle & operator=(nullptr_t) { unref(); return (*this); }										\
																									\
	Handle & operator=(Ref<m_class> const & value) { reset(value); return (*this); }				\
																									\
	template <class U> Handle & operator=(Ref<U> const & value) { reset(value); return (*this); }	\
																									\
	Handle & operator=(m_class const & value) { instance(value); return (*this); }					\
																									\
	Handle & operator=(m_class && value) noexcept { instance(std::move(value)); return (*this); }	\
																									\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// handle
	template <class T> class Handle : public BaseHandle<T>
	{
		ISM_HANDLE_DEFAULT(T);

	public:
		Handle() = default;

		~Handle() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

template <class T> struct ism::Hash<ism::api::Handle<T>>
{
	hash_t operator()(ism::api::Handle<T> const & o) const { return hash(o); }
};

template <class T> struct ism::EqualTo<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.equal_to(b); }
};

template <class T> struct ism::NotEqualTo<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.not_equal_to(b); }
};

template <class T> struct ism::Less<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.less(b); }
};

template <class T> struct ism::Greater<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.greater(b); }
};

template <class T> struct ism::LessEqual<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.less_equal(b); }
};

template <class T> struct ism::GreaterEqual<ism::api::Handle<T>>
{
	template <class U> bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.greater_equal(b); }
};

// object common
#define ISM_OBJECT_COMMON(m_class, m_inherits)														\
private:																							\
	using self_type = m_class;																		\
																									\
	using base_type = m_inherits;																	\
																									\
	friend class ism::api::ClassDB;																	\
																									\
	friend class ism::api::Handle<m_class>;															\
																									\
	static ism::api::TypeObject ob_type_static;														\
																									\
protected:																							\
	static void initialize_class()																	\
	{																								\
		if (static bool once{}; !once && (once = true))												\
		{																							\
			ism::api::ClassDB::add_class<m_class>();												\
																									\
			if (m_class::_get_bind_methods() != m_inherits::_get_bind_methods())					\
			{																						\
				m_class::_bind_methods(m_class::ob_type_static);									\
			}																						\
		};																							\
	}																								\
																									\
	virtual void _initialize_classv() override														\
	{																								\
		m_class::initialize_class();																\
	}																								\
																									\
	FORCE_INLINE static constexpr void (*_get_bind_methods())(ism::api::TypeObject & t)				\
	{																								\
		return &m_class::_bind_methods;																\
	}																								\
																									\
	FORCE_INLINE virtual ism::api::TypeObject * _get_typev() const override							\
	{																								\
		return m_class::get_type_static();															\
	}																								\
																									\
public:																								\
	explicit m_class(ism::api::TypeObject const * t) noexcept : m_inherits{ t } {}					\
																									\
	FORCE_INLINE static ism::api::TypeObject * get_type_static()									\
	{																								\
		return &m_class::ob_type_static;															\
	}																								\
																									\
public:																								\
	NODISCARD virtual String get_class() const override												\
	{																								\
		return String{ TOSTR(m_class) };															\
	}																								\
																									\
	NODISCARD FORCE_INLINE static void * get_class_ptr_static()										\
	{																								\
		static int32_t ptr; return &ptr;															\
	}																								\
																									\
	NODISCARD FORCE_INLINE static constexpr StringView get_class_static()							\
	{																								\
		return TOSTR(m_class);																		\
	}																								\
																									\
	NODISCARD FORCE_INLINE static constexpr StringView get_parent_class_static()					\
	{																								\
		return m_inherits::get_class_static();														\
	}																								\
																									\
	NODISCARD static constexpr StringView inherits_static()											\
	{																								\
		return TOSTR(m_inherits);																	\
	}																								\
																									\
	NODISCARD virtual bool is_class(String const & value) const override							\
	{																								\
		return (value == TOSTR(m_class)) ? true : m_inherits::is_class(value);						\
	}																								\
																									\
	NODISCARD virtual bool is_class_ptr(void * value) const override								\
	{																								\
		return (value == m_class::get_class_ptr_static()) ? true : m_inherits::is_class_ptr(value);	\
	}																								\
																									\
private:

// object cvt
#define ISM_OBJECT_CVT(m_class, m_inherits)					\
	ISM_OBJECT_COMMON(m_class, m_inherits)					\
public:														\
	COPYABLE_MOVABLE(m_class)								\
private:

// object default
#define ISM_OBJECT_DEFAULT(m_class, m_inherits)				\
	ISM_OBJECT_CVT(m_class, m_inherits);					\
public:														\
	m_class() : m_inherits{ &m_class::ob_type_static } {}	\
private:

// object
namespace ism::api
{
	class ISM_API BaseObject : public ObjectAPI<BaseObject>, public Reference
	{
	private:
		friend class ClassDB;

		friend class Handle<BaseObject>;

		static TypeObject ob_type_static; // class type

	protected:
		mutable Ref<TypeObject> ob_type; // instance type

		static void initialize_class();

		virtual void _initialize_classv() { initialize_class(); }

		static void _bind_methods(TypeObject & t);

		FORCE_INLINE static constexpr void (*_get_bind_methods())(TypeObject &) { return &BaseObject::_bind_methods; }

		FORCE_INLINE virtual TypeObject * _get_typev() const { return get_type_static(); }

		explicit BaseObject(TypeObject const * t);

	public:
		virtual ~BaseObject() override { ob_type = nullptr; }

		NODISCARD static TypeObject * get_type_static() { return &ob_type_static; }

		NODISCARD TypeObject * get_type() const { if (!ob_type) { ob_type = _get_typev(); } return *ob_type; }

		NODISCARD BaseObject * ptr() const { return const_cast<BaseObject *>(this); }

		template <class T> NODISCARD T cast() const &; // cast.hpp

		template <class T> NODISCARD T cast() &&; // cast.hpp

	public:
		NODISCARD virtual String get_class() const { return "BaseObject"; }

		NODISCARD static void * get_class_ptr_static() { static int32_t ptr; return &ptr; }

		NODISCARD static constexpr StringView get_class_static() { return "BaseObject"; }

		NODISCARD static constexpr StringView get_parent_class_static() { return ""; }

		NODISCARD virtual bool is_class(String const & value) const { return value == "BaseObject"; }

		NODISCARD virtual bool is_class_ptr(void * value) const { return value == get_class_ptr_static(); }
	};
}

// object deleter
namespace ism
{
	template <> struct DefaultDelete<api::BaseObject>
	{
		template <class U> void operator()(U * ptr) const
		{
			if (auto t{ api::typeof(ptr) }; t && t->tp_free)
			{
				t->tp_free(ptr);
			}
			else
			{
				memdelete(ptr);
			}
		}
	};
}

// object handle
namespace ism::api
{
	template <> class Handle<BaseObject> : public BaseHandle<BaseObject>
	{
		ISM_HANDLE_DEFAULT(BaseObject);

	public:
		Handle() = default;

		~Handle() = default;
	};
}

// accessors
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// accessor
	template <class Policy
	> class Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using key_type = typename Policy::key_type;

		Accessor(OBJECT obj, key_type key) : m_obj{ obj }, m_key{ key } {}
		Accessor(Accessor const &) = default;
		Accessor(Accessor &&) noexcept = default;
		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT(a)); }
		void operator=(Accessor const & a) & { operator=(OBJECT(a)); }

		template <class Value> decltype(auto) operator=(Value && value) &&
		{
			Policy::set(m_obj, m_key, FWD_OBJ(value));
			return (*this);
		}

		template <class Value> decltype(auto) operator=(Value && value) &
		{
			get_cache() = FWD_OBJ(value);
			return (*this);
		}

		NODISCARD auto ptr() const { return const_cast<BaseObject *>(get_cache().ptr()); }

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

	// attribute accessor
	template <class T> struct accessor_policies::Attr
	{
		using key_type = T;

		template <class O = OBJECT, class Index = T
		> static OBJECT get(O && o, Index && i) { return getattr(FWD(o), FWD(i)); }

		template <class O = OBJECT, class Index = T, class Value = OBJECT
		> static void set(O && o, Index && i, Value && v) { setattr(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// item accessor
	template <class T> struct accessor_policies::Item
	{
		using key_type = T;

		template <class O = OBJECT, class Index = T
		> static OBJECT get(O && o, Index && i) { return getitem(FWD(o), FWD(i)); }

		template <class O = OBJECT, class Index = T, class Value = OBJECT
		> static void set(O && o, Index && i, Value && v) { setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// functions
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJECT) generic_getattr_with_dict(OBJECT obj, OBJECT name, OBJECT dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJECT obj, OBJECT name, OBJECT value, OBJECT dict);

	inline OBJECT generic_getattr(OBJECT obj, OBJECT name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJECT obj, OBJECT name, OBJECT value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD OBJECT object_or_cast(T && o);

	NODISCARD inline OBJECT object_or_cast(BaseObject * o) { return OBJECT{ o }; }

	NODISCARD inline OBJECT object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline TYPE typeof_generic(std::type_info const & t); // cast.hpp

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		if constexpr (is_ref_v<T>)
		{
			return typeof<typename T::value_type>();
		}
		else
		{
			return T::get_type_static();
		}
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return typeof_generic(typeid(T));
	}

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return o ? o->get_type() : nullptr;
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return typeof(FWD_OBJ(o));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline bool isinstance_generic(OBJECT const & o, std::type_info const & t); // cast.hpp

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return typeof(o).is(typeof<T>());
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return isinstance_generic(o, typeid(T));
	}

	template <class A, class B = A
	> NODISCARD bool isinstance(A const & a, B const & b)
	{
		return (a && b) && (typeof(a) == (isinstance<TYPE>(b) ? b : typeof(b)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, Index && i)
	{
		if (TYPE t{ typeof(o) }; !t)
		{
			return nullptr;
		}
		else if constexpr (mpl::is_string_v<Index>)
		{
			if (t->tp_getattr) { return t->tp_getattr(o, FWD(i)); }
			else if (t->tp_getattro) { return t->tp_getattro(o, FWD_OBJ(i)); }
			else { return nullptr; }
		}
		else
		{
			if (t->tp_getattro) { return t->tp_getattro(o, FWD_OBJ(i)); }
			else if (t->tp_getattr) { return t->tp_getattr(o, STR(FWD(i)).c_str()); }
			else { return nullptr; }
		}
	}

	template <class Index = OBJECT, class Value = OBJECT
	> Error setattr(OBJECT o, Index && i, Value && v)
	{
		if (TYPE t{ typeof(o) }; !t)
		{
			return Error_Unknown;
		}
		else if constexpr (mpl::is_string_v<Index>)
		{
			if (t->tp_setattr) { return t->tp_setattr(o, FWD(i), FWD_OBJ(v)); }
			else if (t->tp_getattro) { return t->tp_setattro(o, FWD_OBJ(i), FWD_OBJ(v)); }
			else { return Error_Unknown; }
		}
		else
		{
			if (t->tp_getattro) { return t->tp_setattro(o, FWD_OBJ(i), FWD_OBJ(v)); }
			else if (t->tp_setattr) { return t->tp_setattr(o, STR(FWD(i)).c_str(), FWD_OBJ(v)); }
			else { return Error_Unknown; }
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJECT
	> NODISCARD OBJECT getitem(OBJECT o, Index && i)
	{
		if (!o.is_valid()) { return nullptr; }
		else if constexpr (mpl::is_string_v<Index>) { return DICT(o)[FWD(i)]; }
		else if constexpr (std::is_integral_v<Index>) { return LIST(o)[FWD(i)]; }
		else if (isinstance<DICT>(o)) { return DICT(o)[FWD(i)]; }
		else if (isinstance<LIST>(o)) { return LIST(o)[FWD(i)]; }
		else { return nullptr; }
	}

	template <class Index = OBJECT, class Value = OBJECT
	> Error setitem(OBJECT o, Index && i, Value && v)
	{
		if (!o.is_valid()) { return Error_Unknown; }
		else if constexpr (mpl::is_string_v<Index>) { return DICT(o).set(FWD(i), FWD(v)); }
		else if constexpr (std::is_integral_v<Index>) { return LIST(o).set(FWD(i), FWD(v)); }
		else if (isinstance<DICT>(o)) { return DICT(o).set(FWD(i), FWD(v)); }
		else if (isinstance<LIST>(o)) { return LIST(o).set(FWD(i), FWD(v)); }
		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
