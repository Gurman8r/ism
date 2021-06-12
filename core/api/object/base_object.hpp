#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/detail/common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object_api
template <class Derived
> class ism::api::ObjectAPI : _API_Tag
{
private:
	NODISCARD auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

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
> NODISCARD bool operator==(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.equal_to(b); }

template <class T
> NODISCARD bool operator!=(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.not_equal_to(b); }

template <class T
> NODISCARD bool operator<(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.less(b); }

template <class T
> NODISCARD bool operator<=(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.less_equal(b); }

template <class T
> NODISCARD bool operator>(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.greater(b); }

template <class T
> NODISCARD bool operator>=(ism::api::ObjectAPI<T> const & a, ism::api::ObjectAPI<T> const & b) { return a.greater_equal(b); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// base handle
template <class T> class ism::api::BaseHandle : public ObjectAPI<Handle<T>>, public Ref<T>
{
protected:
	BaseHandle() = default;

public:
	~BaseHandle() = default;

	template <class T> NODISCARD T cast() const &;

	template <class T> NODISCARD T cast() &&;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// handle common
#define ISM_HANDLE(m_class)																			\
public:																								\
	Handle(nullptr_t) {}																			\
																									\
	Handle(m_class * value) { if (value) { ref_pointer(value); } }									\
																									\
	Handle(Ref<m_class> const & value) { ref(value); }												\
																									\
	template <class U> Handle(Ref<U> const & value) { reset(value); }								\
																									\
	Handle(m_class const & value) { revalue(value); }												\
																									\
	Handle(m_class && value) noexcept { revalue(std::move(value)); }								\
																									\
	Handle & operator=(nullptr_t) { unref(); return (*this); }										\
																									\
	Handle & operator=(Ref<m_class> const & value) { reset(value); return (*this); }				\
																									\
	template <class U> Handle & operator=(Ref<U> const & value) { reset(value); return (*this); }	\
																									\
	Handle & operator=(m_class const & value) { revalue(value); return (*this); }					\
																									\
	Handle & operator=(m_class && value) noexcept { revalue(std::move(value)); return (*this); }	\
																									\
	template <class ... Args																		\
	> NODISCARD static Handle<m_class> create(Args && ... args) noexcept							\
	{																								\
		return { memnew(m_class{ FWD(args)... }) };													\
	}																								\
																									\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// handle
template <class T> class ism::api::Handle : public BaseHandle<T>
{
	ISM_HANDLE(T);

public:
	Handle() = default;

	~Handle() = default;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <class T> struct ism::Hash<ism::api::Handle<T>>
{
	NODISCARD hash_t operator()(ism::api::Handle<T> const & o) const { return hash(o); }
};

template <class T> struct ism::EqualTo<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.equal_to(b); }
};

template <class T> struct ism::NotEqualTo<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.not_equal_to(b); }
};

template <class T> struct ism::Less<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.less(b); }
};

template <class T> struct ism::Greater<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.greater(b); }
};

template <class T> struct ism::LessEqual<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.less_equal(b); }
};

template <class T> struct ism::GreaterEqual<ism::api::Handle<T>>
{
	template <class U
	> NODISCARD bool operator()(ism::api::Handle<T> const & a, ism::api::Handle<U> const & b) const { return a.greater_equal(b); }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object class
#define ISM_OBJECT_CLASS(m_class, m_inherits)											\
	ISM_SUPER_CLASS(m_class, m_inherits)												\
																						\
private:																				\
	friend class ism::api::ClassDB;														\
																						\
	friend class ism::api::Handle<m_class>;												\
																						\
	static ism::api::TypeObject ob_type_static;											\
																						\
protected:																				\
	virtual void _initialize_classv() override											\
	{																					\
		m_class::initialize_class();													\
	}																					\
																						\
	FORCE_INLINE static constexpr void (*_get_bind_methods())(ism::api::TypeObject & t)	\
	{																					\
		return &m_class::_bind_methods;													\
	}																					\
																						\
	NODISCARD FORCE_INLINE virtual ism::api::TypeObject * _get_typev() const override	\
	{																					\
		return m_class::get_type_static();												\
	}																					\
																						\
public:																					\
	static void initialize_class()														\
	{																					\
		static SCOPE_ENTER(&)															\
		{																				\
			ism::api::ClassDB::add_class<m_class>();									\
																						\
			if (m_class::_get_bind_methods() != m_inherits::_get_bind_methods())		\
			{																			\
				m_class::_bind_methods(m_class::ob_type_static);						\
			}																			\
		};																				\
	}																					\
																						\
	NODISCARD FORCE_INLINE static ism::api::TypeObject * get_type_static()				\
	{																					\
		return &m_class::ob_type_static;												\
	}																					\
																						\
public:																					\
	explicit m_class(ism::api::TypeObject const * t) noexcept : m_inherits{ t } {}		\
																						\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object cvt
#define ISM_OBJECT_CVT(m_class, m_inherits)					\
	ISM_OBJECT_CLASS(m_class, m_inherits)					\
public:														\
	COPY_AND_MOVE_CONSTRUCTABLE(m_class)					\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define ISM_OBJECT(m_class, m_inherits)						\
	ISM_OBJECT_CVT(m_class, m_inherits);					\
public:														\
	m_class() : m_inherits{ &m_class::ob_type_static } {}	\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// base object
class NODISCARD ISM_API ism::api::BaseObject : public Reference, public ObjectAPI<ism::api::BaseObject>
{
	ISM_SUPER_CLASS(BaseObject, Reference);

private:
	friend class ClassDB;

	friend class Handle<BaseObject>;

	static TypeObject ob_type_static; // static class type

protected:
	mutable Ref<TypeObject> ob_type; // instance class type

	virtual void _initialize_classv() { initialize_class(); }

	static void _bind_methods(TypeObject & t);

	NODISCARD FORCE_INLINE static constexpr void (*_get_bind_methods())(TypeObject &) { return &BaseObject::_bind_methods; }

	NODISCARD FORCE_INLINE virtual TypeObject * _get_typev() const { return get_type_static(); }

	explicit BaseObject(TypeObject const * t) noexcept : ob_type{ const_cast<TypeObject *>(t) } {}

public:
	virtual ~BaseObject() override { ob_type = nullptr; }

	static void initialize_class();

	FORCE_INLINE static void register_custom_data() {}

	NODISCARD static TypeObject * get_type_static() { return &ob_type_static; }

	NODISCARD TypeObject * get_type() const { if (!ob_type) { ob_type = _get_typev(); } return *ob_type; }

	NODISCARD BaseObject * ptr() const { return const_cast<BaseObject *>(this); }

	template <class T> NODISCARD T cast() const &;

	template <class T> NODISCARD T cast() &&;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object deleter
template <> struct ism::DefaultDelete<ism::api::BaseObject>
{
	template <class U> void operator()(U * ptr) const
	{
		using namespace api;

		if (TYPE t{ typeof(ptr) }; t && t->tp_free)
		{
			t->tp_free(ptr);
		}
		else
		{
			memdelete(ptr);
		}
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object handle
template <> class ism::api::Handle<ism::api::BaseObject> : public BaseHandle<BaseObject>
{
	ISM_HANDLE(BaseObject);

public:
	Handle() = default;

	~Handle() = default;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// accessor
template <class Policy
> class ism::api::Accessor : public ObjectAPI<ism::api::Accessor<Policy>>
{
public:
	using key_type = typename Policy::key_type;

	Accessor(OBJECT obj, key_type key) : m_obj{ obj }, m_key{ key } {}
		
	Accessor(Accessor const &) = default;
	Accessor(Accessor &&) noexcept = default;

	void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT(a)); }
	void operator=(Accessor const & a) & { operator=(OBJECT(a)); }

	template <class T> decltype(auto) operator=(T && v) &&
	{
		Policy::set(m_obj, m_key, object_or_cast(FWD(v)));
		return (*this);
	}

	template <class T> decltype(auto) operator=(T && v) &
	{
		get_cache() = object_or_cast(FWD(v));
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// attr accessor
template <class T> struct ism::api::accessor_policies::Attr
{
	using key_type = T;

	template <class O = OBJECT, class Index = T
	> static OBJECT get(O && o, Index && i) { return getattr(FWD(o), FWD(i)); }

	template <class O = OBJECT, class Index = T, class Value = OBJECT
	> static void set(O && o, Index && i, Value && v) { setattr(FWD(o), FWD(i), FWD(v)); }
};

// item accessor
template <class T> struct ism::api::accessor_policies::Item
{
	using key_type = T;

	template <class O = OBJECT, class Index = T
	> static OBJECT get(O && o, Index && i) { return getitem(FWD(o), FWD(i)); }

	template <class O = OBJECT, class Index = T, class Value = OBJECT
	> static void set(O && o, Index && i, Value && v) { setitem(FWD(o), FWD(i), FWD(v)); }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// functions
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline TYPE typeof_generic(std::type_info const & t);

	NODISCARD inline bool isinstance_generic(OBJECT const & o, std::type_info const & t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD OBJECT object_or_cast(T && o);

	NODISCARD inline OBJECT object_or_cast(BaseObject * o) { return OBJECT{ o }; }

	NODISCARD inline OBJECT object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		return api::typeof_generic(typeid(T));
	}

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return o ? o->get_type() : nullptr;
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return typeof(object_or_cast(FWD(o)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return typeof(o) == typeof<T>();
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return api::isinstance_generic(o, typeid(T));
	}

	template <class A, class B = A
	> NODISCARD bool isinstance(A const & a, B const & b)
	{
		return typeof(a) == (isinstance<TYPE>(b) ? b : typeof(b));
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
			if (t->tp_getattr)
			{
				return t->tp_getattr(o, FWD(i));
			}
			else if (t->tp_getattro)
			{
				return t->tp_getattro(o, object_or_cast(FWD(i)));
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
				return t->tp_getattro(o, object_or_cast(FWD(i)));
			}
			else if (t->tp_getattr)
			{
				return t->tp_getattr(o, STR(FWD(i)).c_str());
			}
			else
			{
				return nullptr;
			}
		}
	}

	template <class Index = OBJECT, class Defval = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, Index && i, Defval && defval) noexcept
	{
		OBJECT result{ getattr(o, FWD(i)) };
		return result ? result : object_or_cast(FWD(defval));
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
			if (t->tp_setattr)
			{
				return t->tp_setattr(o, FWD(i), object_or_cast(FWD(v)));
			}
			else if (t->tp_getattro)
			{
				return t->tp_setattro(o, object_or_cast(i), object_or_cast(FWD(v)));
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
				return t->tp_setattro(o, object_or_cast(FWD(i)), object_or_cast(FWD(v)));
			}
			else if (t->tp_setattr)
			{
				return t->tp_setattr(o, STR(FWD(i)).c_str(), object_or_cast(FWD(v)));
			}
			else
			{
				return Error_Unknown;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJECT
	> NODISCARD OBJECT getitem(OBJECT o, Index && i)
	{
		if (!o.is_valid())
		{
			return nullptr;
		}
		else if constexpr (mpl::is_string_v<Index>)
		{
			return DICT(o).get(FWD(i));
		}
		else if constexpr (std::is_integral_v<Index>)
		{
			return LIST(o).get(FWD(i));
		}
		else if (isinstance<DICT>(o))
		{
			return DICT(o).get(FWD(i));
		}
		else if (isinstance<LIST>(o))
		{
			return LIST(o).get(FWD(i));
		}
		else
		{
			return nullptr;
		}
	}

	template <class Index = OBJECT, class Value = OBJECT
	> Error setitem(OBJECT o, Index && i, Value && v)
	{
		if (!o.is_valid())
		{
			return Error_Unknown;
		}
		else if constexpr (mpl::is_string_v<Index>)
		{
			return DICT(o).set(FWD(i), FWD(v));
		}
		else if constexpr (std::is_integral_v<Index>)
		{
			return LIST(o).set(FWD(i), FWD(v));
		}
		else if (isinstance<DICT>(o))
		{
			return DICT(o).set(FWD(i), FWD(v));
		}
		else if (isinstance<LIST>(o))
		{
			return LIST(o).set(FWD(i), FWD(v));
		}
		else
		{
			return Error_Unknown;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// class_db
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ClassDB final
	{
	public:
		static Batch<
			hash_t,
			StringName,
			TYPE
		> classes;

	public:
		static void add_class(StringName const & name, TypeObject * type);

		template <class T> static void add_class()
		{
			add_class(T::get_class_static(), T::get_type_static());
		}

		template <class T> static void register_class()
		{
			T::initialize_class();

			TYPE * type{ classes.map<hash_t, TYPE>(hash(T::get_class_static())) };

			VERIFY(type && *type);

			(*type)->m_class_ptr = (StringName *)T::get_class_ptr_static();

			T::register_custom_data();
		}

		NODISCARD static bool class_exists(StringName const & class_name);

		NODISCARD static TYPE class_type(StringName const & class_name);

		template <class T> NODISCARD static TYPE class_type() { return class_type(T::get_class_static()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
