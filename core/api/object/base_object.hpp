#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/common.hpp>

// object common
#define ISM_OBJECT_COMMON(m_class, m_inherits)											\
ISM_SUPER(m_class, m_inherits)															\
private:																				\
	using self_type = m_class;															\
																						\
	using base_type = m_inherits;														\
																						\
	friend class ism::api::TypeDB;														\
																						\
	friend class ism::api::Handle<m_class>;												\
																						\
	static ism::api::TypeObject ob_type_static;											\
																						\
protected:																				\
	static void initialize_class()														\
	{																					\
		if (static bool once{}; !once && (once = true))									\
		{																				\
			ism::api::TypeDB::add_class<m_class>();										\
																						\
			if (m_class::_get_bind_class() != m_inherits::_get_bind_class())			\
			{																			\
				m_class::_bind_class(m_class::ob_type_static);							\
			}																			\
		};																				\
	}																					\
																						\
	virtual void _initialize_classv() override											\
	{																					\
		m_class::initialize_class();													\
	}																					\
																						\
	FORCE_INLINE static constexpr void (*_get_bind_class())(ism::api::TypeObject & t)	\
	{																					\
		return &m_class::_bind_class;													\
	}																					\
																						\
	FORCE_INLINE virtual ism::api::TypeObject * _get_typev() const override				\
	{																					\
		return m_class::get_type_static();												\
	}																					\
																						\
public:																					\
	explicit m_class(ism::api::TypeObject const * t) noexcept : m_inherits{ t } {}		\
																						\
	FORCE_INLINE static ism::api::TypeObject * get_type_static()						\
	{																					\
		return &m_class::ob_type_static;												\
	}																					\
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
	class ISM_API BaseObject : public Reference, public ObjectAPI<BaseObject>
	{
		ISM_SUPER(BaseObject, Reference);

		friend class TypeDB;

		friend class Handle<BaseObject>;

		static TypeObject ob_type_static; // static class type

	protected:
		static void initialize_class();

		virtual void _initialize_classv() { initialize_class(); }

		static void _bind_class(TypeObject & t);

		FORCE_INLINE static constexpr void (*_get_bind_class())(TypeObject &) { return &BaseObject::_bind_class; }

		FORCE_INLINE virtual TypeObject * _get_typev() const { return get_type_static(); }

		explicit BaseObject(TypeObject const * t) noexcept : ob_type{ (TypeObject *)t } {}

		mutable Ref<TypeObject> ob_type; // instance type

	public:
		virtual ~BaseObject() override { ob_type = nullptr; }

		NODISCARD BaseObject * ptr() const { return const_cast<BaseObject *>(this); }

		NODISCARD static TypeObject * get_type_static() { return &ob_type_static; }

		NODISCARD TypeObject * get_type() const { if (!ob_type) { ob_type = _get_typev(); } return *ob_type; }

		bool set_type(TypeObject * value) { return (ob_type = value); }

		template <class T> NODISCARD T cast() const &; // cast.hpp

		template <class T> NODISCARD T cast() &&; // cast.hpp
	};
}

// object delete
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

// object check
#define ISM_OBJECT_CHECK(o) (true)

// object handle
namespace ism::api
{
	template <> class Handle<BaseObject> : public BaseHandle<BaseObject>
	{
		ISM_HANDLE_DEFAULT(BaseObject, ISM_OBJECT_CHECK);

	public:
	};
}

// functions
namespace ism::api
{
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

	template <class T, class O = OBJECT, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		TYPE const a{ typeof(FWD(o)) }, b{ typeof<T>() };

		return a && b && (a.is(b) || a.is_subtype(b));
	}

	template <class T, class O = OBJECT, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		return isinstance_generic(FWD(o), typeid(T));
	}

	template <class A = OBJECT, class B = OBJECT
	> NODISCARD bool isinstance(A const & lhs, B const & rhs)
	{
		TYPE const a{ typeof(lhs) }, b{ TYPE::check_(rhs) ? rhs : typeof(rhs) };

		return a && b && (a.is(b) || a.is_subtype(b));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, Index && i)
	{
		if (TYPE t; !o || !(t = typeof(o)))
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

	template <class Index = OBJECT
	> NODISCARD OBJECT getattr(OBJECT o, Index && i, OBJECT defval)
	{
		OBJECT str_name{ FWD_OBJ(i) };
		return hasattr(o, str_name) ? getattr(o, str_name) : nullptr;
	}

	template <class Index = OBJECT, class Value = OBJECT
	> Error setattr(OBJECT o, Index && i, Value && v)
	{
		if (TYPE t; !o || !(t = typeof(o)))
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

	template <class Index = OBJECT
	> NODISCARD bool hasattr(OBJECT o, Index && i)
	{
		return object_hasattr(o, FWD_OBJ(i));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJECT
	> NODISCARD OBJECT getitem(OBJECT o, Index && i)
	{
		if (!o.is_valid()) { return nullptr; }
		else if constexpr (mpl::is_string_v<Index>) { return DICT(o)[FWD(i)]; }
		else if constexpr (std::is_integral_v<Index>) { return LIST(o)[FWD(i)]; }
		else if (DICT::check_(o)) { return DICT(o)[FWD(i)]; }
		else if (LIST::check_(o)) { return LIST(o)[FWD(i)]; }
		else { return nullptr; }
	}

	template <class Index = OBJECT, class Value = OBJECT
	> Error setitem(OBJECT o, Index && i, Value && v)
	{
		if (!o.is_valid()) { return Error_Unknown; }
		else if constexpr (mpl::is_string_v<Index>) { return DICT(o).set(FWD(i), FWD(v)); }
		else if constexpr (std::is_integral_v<Index>) { return LIST(o).set(FWD(i), FWD(v)); }
		else if (DICT::check_(o)) { return DICT(o).set(FWD(i), FWD(v)); }
		else if (LIST::check_(o)) { return LIST(o).set(FWD(i), FWD(v)); }
		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJECT) object_alloc(TYPE type);

	ISM_API_FUNC(Error) object_init(OBJECT self, OBJECT args);

	ISM_API_FUNC(OBJECT) object_new(TYPE type, OBJECT args);

	ISM_API_FUNC(bool) object_hasattr(OBJECT obj, OBJECT name);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJECT) generic_getattr_with_dict(OBJECT obj, OBJECT name, OBJECT dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJECT obj, OBJECT name, OBJECT value, OBJECT dict);

	inline OBJECT generic_getattr(OBJECT obj, OBJECT name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJECT obj, OBJECT name, OBJECT value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
