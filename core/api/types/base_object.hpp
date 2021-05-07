#ifndef _ISM_API_TYPES_BASE_OBJECT_HPP_
#define _ISM_API_TYPES_BASE_OBJECT_HPP_

#include <core/api/detail/operators.hpp>

// none
#define Core_None (ism::OBJECT(&ism::CoreObject::g_none))

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
		> NODISCARD bool contains(V && v) const { return attr("__contains__")(FWD(v)).cast<bool>(); }

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJECT operator()(Args && ... args); // call.hpp

		NODISCARD bool is_none() const noexcept { return derived().ptr() == *Core_None; }
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

		NODISCARD auto type() const
		{
			auto * o{ CHECK(derived().ptr()) };

			TYPE & t{ o->ob_type };

			if (!t) { t.reset(o->_get_typev()); }

			return t;
		}

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
}

// handle
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> class Handle : public ObjectAPI<Handle<T>>, public Ref<T>
	{
	public:
		~Handle() { destruct(m_reference); m_reference = nullptr; }

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

	public:
		static void initialize_class() noexcept { T::initialize_class(); }

		NODISCARD static TYPE type_static() noexcept { return T::type_static(); }

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;

	protected:
		static void destruct(T * ptr)
		{
			if (ptr && ptr->dec_ref())
			{
				if (TYPE & t{ ptr->ob_type }; t && t->tp_operator_delete)
				{
					t->tp_operator_delete(ptr);
				}
				else
				{
					memdelete(ptr);
				}
			}
		}
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
			Policy::set(m_obj, m_key, detail::object_forward(FWD(v)));
			return (*this);
		}

		template <class T> decltype(auto) operator=(T && v) &
		{
			get_cache() = detail::object_forward(FWD(v));
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
		ISM_SUPER_CLASS(CoreObject, Reference);

	private:
		template <class T> friend class ObjectAPI;
		template <class T> friend class Handle;

		static CoreType ob_class; // object class

	protected:
		mutable TYPE ob_type; // object type

		virtual bool _predelete() override;

		virtual void _postinitialize() override;

		virtual void _initialize_classv() { initialize_class(); }

		static void _bind_class(CoreType & t);

		NODISCARD static constexpr void (*_get_bind_class())(CoreType &) { return &CoreObject::_bind_class; }

		virtual TYPE _get_typev() { return type_static(); }

	public:
		static CoreObject g_none; // global none

		virtual ~CoreObject() override { ob_type = nullptr; }

		explicit CoreObject(TYPE const & t) { ob_type = t; }

	public:
		static void initialize_class();

		static void register_custom_data() {}

		NODISCARD static TYPE type_static() { return TYPE(&ob_class); }

		NODISCARD auto ptr() const { return const_cast<CoreObject *>(this); }

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define ISM_OBJECT_COMMON(m_class, m_inherits)												\
	ISM_SUPER_CLASS(m_class, m_inherits)													\
private:																					\
	template <class T> friend class ObjectAPI;												\
	template <class T> friend class Handle;													\
																							\
	static CoreType ob_class;																\
																							\
public:																						\
	NODISCARD static TYPE type_static() { return TYPE(&m_class::ob_class); }				\
																							\
	NODISCARD static void initialize_class()												\
	{																						\
		static SCOPE_ENTER(&)																\
		{																					\
			ClassDB::add_class<m_class>();													\
																							\
			if (m_class::_get_bind_class() != m_inherits::_get_bind_class())				\
			{																				\
				m_class::_bind_class(m_class::ob_class);									\
			}																				\
		};																					\
	}																						\
																							\
protected:																					\
	NODISCARD virtual TYPE _get_typev() override { return m_class::type_static(); }			\
																							\
	virtual void _initialize_classv() override { m_class::initialize_class(); }				\
																							\
	FORCE_INLINE static constexpr void (*_get_bind_class())(CoreType & t)					\
	{																						\
		return &m_class::_bind_class;														\
	}																						\
																							\
public:																						\
	explicit m_class(TYPE const & t) : m_inherits{ t } {}									\
																							\
private:

// object cvt
#define ISM_OBJECT_CVT(m_class, m_inherits)													\
	ISM_OBJECT_COMMON(m_class, m_inherits)													\
public:																						\
	m_class(m_class const &) = default;														\
	m_class(m_class &&) noexcept = default;													\
	m_class & operator=(m_class const &) = default;											\
	m_class & operator=(m_class &&) noexcept = default;										\
																							\
private:

// object default
#define ISM_OBJECT_DEFAULT(m_class, m_inherits)												\
	ISM_OBJECT_CVT(m_class, m_inherits);													\
public:																						\
	virtual ~m_class() noexcept override = default;											\
																							\
	m_class() : m_inherits{ m_class::type_static() } {}										\
																							\
private:

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
		static void add_class(StringName const & name, TYPE t);

		template <class T> static void add_class()
		{
			add_class(T::get_class_static(), T::type_static());
		}

		template <class T> static void register_type()
		{
			using U = std::conditional_t<is_handle_v<T>, typename T::value_type, mpl::intrinsic_t<T>>;

			U::initialize_class();

			TYPE * t{ classes.map<hash_t, TYPE>(hash(U::get_class_static())) };
			VERIFY(t);

			U::register_custom_data();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// helper functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
		> NODISCARD auto object_forward(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

		template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
		> NODISCARD OBJECT object_forward(T && o);

		NODISCARD inline OBJECT object_forward(CoreObject * o) { return OBJECT{ o }; }

		NODISCARD inline OBJECT object_forward(cstring s) { return object_forward(String{ s }); }

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
		return o ? o->type() : nullptr;
	}

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return typeof(detail::object_forward(FWD(o)));
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
		else if constexpr (mpl::is_string_v<I>)
		{
			if (t->tp_getattr)
			{
				return t->tp_getattr(o, FWD(i));
			}
			else if (t->tp_getattro)
			{
				return t->tp_getattro(o, detail::object_forward(FWD(i)));
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
				return t->tp_getattro(o, detail::object_forward(FWD(i)));
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
		return result ? result : detail::object_forward(FWD(defval));
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
		else if constexpr (mpl::is_string_v<I>)
		{
			if (t->tp_setattr)
			{
				return t->tp_setattr(o, FWD(i), detail::object_forward(FWD(v)));
			}
			else if (t->tp_getattro)
			{
				return t->tp_setattro(o, detail::object_forward(i), detail::object_forward(FWD(v)));
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
				return t->tp_setattro(o, detail::object_forward(FWD(i)), detail::object_forward(FWD(v)));
			}
			else if (t->tp_setattr)
			{
				return t->tp_setattr(o, STR(FWD(i))->c_str(), detail::object_forward(FWD(v)));
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
			return (***DICT(o))[detail::object_forward(FWD(i))];
		}
		else if (isinstance<LIST>(o))
		{
			return (***LIST(o))[detail::object_forward(FWD(i)).cast<size_t>()];
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
			return ((***DICT(o))[detail::object_forward(FWD(i))] = detail::object_forward(FWD(v))), Error_None;
		}
		else if (isinstance<LIST>(o))
		{
			return ((***LIST(o))[detail::object_forward(FWD(i)).cast<size_t>()] = detail::object_forward(FWD(v))), Error_None;
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

#endif // !_ISM_API_TYPES_BASE_OBJECT_HPP_
