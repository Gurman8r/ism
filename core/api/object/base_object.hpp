#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/detail/common.hpp>

// object_api
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
		> NODISCARD bool contains(V && v) const { return attr("__contains__")(handle(), FWD(v)); }

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

	template <class T> class BaseHandle : public ObjectAPI<Handle<T>>, public Ref<T>
	{
	public:
		~BaseHandle()
		{
			if (dec_ref())
			{
				if (TYPE t{ typeof(m_ref) }; t && t->tp_free)
				{
					t->tp_free(m_ref);
				}
				else
				{
					memdelete(m_ref);
				}
			}
			m_ref = nullptr;
		}

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
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> class Handle : public BaseHandle<T>
	{
		ISM_HANDLE(T);

	public:
		~Handle() = default;

		Handle() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Handle<T>>
	{
		NODISCARD hash_t operator()(Handle<T> const & o) const { return hash(o); }
	};

	template <class T> struct EqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreObject : public ObjectAPI<CoreObject>, public Reference
	{
		ISM_SUPER(CoreObject, Reference);

	private:
		friend class ClassDB;

		friend class Handle<CoreObject>;

		static CoreType ob_type_static; // class type

	protected:
		mutable Ref<CoreType> ob_type; // instance type

		virtual void _initialize_classv() { initialize_class(); }

		static void _bind_methods(CoreType & t);
		
		NODISCARD static constexpr void (*_get_bind_methods())(CoreType &) { return &CoreObject::_bind_methods; }

		virtual CoreType * _get_typev() const { return get_type_static(); }

		explicit CoreObject(CoreType const * t) : base_type{}, ob_type{ (CoreType *)t } {}

	public:
		virtual ~CoreObject() override {}

		static void initialize_class();

		FORCE_INLINE static void register_custom_data() {}

		NODISCARD static CoreType * get_type_static() { return &ob_type_static; }

		NODISCARD CoreType * get_type() const { if (!ob_type) { ob_type = _get_typev(); } return *ob_type; }

		NODISCARD CoreObject * ptr() const { return const_cast<CoreObject *>(this); }

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreObject> : public BaseHandle<CoreObject>
	{
		ISM_HANDLE(CoreObject);

	public:
		Handle() = default;

		~Handle() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define ISM_OBJECT_CLASS(m_class, m_inherits)										\
	ISM_SUPER(m_class, m_inherits)													\
private:																			\
	friend class ClassDB;															\
																					\
	friend class Handle<m_class>;													\
																					\
	static CoreType ob_type_static;													\
																					\
protected:																			\
	virtual void _initialize_classv() override										\
	{																				\
		m_class::initialize_class();												\
	}																				\
																					\
	FORCE_INLINE static constexpr void (*_get_bind_methods())(CoreType & t)			\
	{																				\
		return &m_class::_bind_methods;												\
	}																				\
																					\
	FORCE_INLINE NODISCARD virtual CoreType * _get_typev() const override			\
	{																				\
		return m_class::get_type_static();											\
	}																				\
																					\
public:																				\
	static void initialize_class()													\
	{																				\
		static SCOPE_ENTER(&)														\
		{																			\
			ClassDB::add_class<m_class>();											\
																					\
			if (m_class::_get_bind_methods() != m_inherits::_get_bind_methods())	\
			{																		\
				m_class::_bind_methods(m_class::ob_type_static);					\
			}																		\
		};																			\
	}																				\
																					\
	FORCE_INLINE NODISCARD static CoreType * get_type_static()						\
	{																				\
		return &m_class::ob_type_static;											\
	}																				\
																					\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object cvt
#define ISM_OBJECT_CVT(m_class, m_inherits)						\
	ISM_OBJECT_CLASS(m_class, m_inherits)						\
public:															\
	explicit m_class(CoreType const * t) : m_inherits{ t } {}	\
	m_class(m_class const &) = default;							\
	m_class(m_class &&) noexcept = default;						\
	m_class & operator=(m_class const &) = default;				\
	m_class & operator=(m_class &&) noexcept = default;			\
																\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object default
#define ISM_OBJECT(m_class, m_inherits)						\
	ISM_OBJECT_CVT(m_class, m_inherits);					\
public:														\
	m_class() : m_inherits{ &m_class::ob_type_static } {}	\
															\
private:

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
			Policy::set(m_obj, m_key, object_or_cast(FWD(v)));
			return (*this);
		}

		template <class T> decltype(auto) operator=(T && v) &
		{
			get_cache() = object_or_cast(FWD(v));
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

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		NODISCARD inline TYPE typeof_generic(std::type_info const & t);

		NODISCARD inline bool isinstance_generic(OBJECT const & o, std::type_info const & t);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD OBJECT object_or_cast(T && o);

	NODISCARD inline OBJECT object_or_cast(CoreObject * o) { return OBJECT{ o }; }

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
		return detail::typeof_generic(typeid(T));
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
namespace ism
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
		static void add_class(StringName const & name, CoreType * type);

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
