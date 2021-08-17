#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/common.hpp>

// object common
#define ISM_OBJECT_COMMON(m_class, m_inherits)										\
private:																			\
	using self_type = typename m_class;												\
																					\
	using base_type = typename m_inherits;											\
																					\
	using holder_type = typename ism::Handle<self_type>;							\
																					\
	friend class ism::TypeDB;														\
																					\
	friend class holder_type;														\
																					\
	static ism::TypeObject _class_type_static;										\
																					\
protected:																			\
	static void initialize_class(ism::OBJ scope)									\
	{																				\
		if (static bool once{}; !once && (once = true))								\
		{																			\
			ism::TypeDB::add_class<m_class>();										\
																					\
			if (m_class::_get_bind_class() != m_inherits::_get_bind_class())		\
			{																		\
				m_class::_bind_class(scope);										\
			}																		\
		};																			\
	}																				\
																					\
	virtual void _initialize_classv(ism::OBJ scope) override						\
	{																				\
		m_class::initialize_class(scope);											\
	}																				\
																					\
	FORCE_INLINE static constexpr void (*_get_bind_class())(ism::OBJ)				\
	{																				\
		return &m_class::_bind_class;												\
	}																				\
																					\
private:

// object default
#define ISM_OBJECT_DEFAULT(m_class, m_inherits)										\
ISM_OBJECT_COMMON(m_class, m_inherits)												\
																					\
protected:																			\
	FORCE_INLINE virtual ism::TYPE _get_typev() const override						\
	{																				\
		return m_class::get_class();												\
	}																				\
																					\
public:																				\
	explicit m_class(ism::TYPE const & t) noexcept : m_inherits{ t } {}				\
																					\
	FORCE_INLINE static ism::TYPE get_class()										\
	{																				\
		return &m_class::_class_type_static;										\
	}																				\
																					\
private:

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object object
	class ISM_API Object : public ObjectAPI<Object>
	{
		using self_type = typename Object;

		using holder_type = typename Handle<self_type>;

		friend class TypeDB;

		friend class holder_type;

		static TypeObject _class_type_static; // static class type

		InstanceID m_instance_id{};

		RefCount m_refcount{}, m_refcount_init{};

		mutable Ref<TypeObject> m_type{};

	protected:
		static void initialize_class(OBJ scope);

		virtual void _initialize_classv(OBJ scope);

		static void _bind_class(OBJ scope);

		FORCE_INLINE static constexpr void (*_get_bind_class())(OBJ) { return &Object::_bind_class; }

		FORCE_INLINE virtual TYPE _get_typev() const;

		explicit Object(TYPE const & type) noexcept;

		virtual void on_inc_ref() {}

		virtual void on_dec_ref() {}

	public:
		virtual ~Object();

		NODISCARD Object * ptr() const noexcept { return const_cast<Object *>(this); }

		template <class T> NODISCARD T cast() const &; // cast.hpp

		template <class T> NODISCARD T cast() &&; // cast.hpp

		NODISCARD static TYPE get_class();

		NODISCARD TYPE get_type() const;

		bool set_type(TYPE const & value);

	public:
		NODISCARD InstanceID get_instance_id() const noexcept { return m_instance_id; }

		NODISCARD int32_t get_ref_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		bool init_ref() noexcept
		{
			if (inc_ref())
			{
				if (!has_references() && m_refcount_init.unref())
				{
					dec_ref(); // first referencing is already 1, so compensate for the ref above
				}
				return true;
			}
			return false;
		}

		bool inc_ref() noexcept // returns false if refcount is at zero and didn't get increased
		{
			uint32_t rc_val{ m_refcount.refval() };
			bool success{ rc_val != 0 };
			if (success && rc_val <= 2 /* higher is not relevant */) { on_inc_ref(); }
			return success;
		}

		bool dec_ref() noexcept
		{
			uint32_t rc_val{ m_refcount.unrefval() };
			bool die{ rc_val == 0 };
			if (rc_val <= 1 /* higher is not relevant */) { on_dec_ref(); }
			return die;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object delete
	template <> struct DefaultDelete<Object>
	{
		template <class U> void operator()(U * ptr) const
		{
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

	// object check
#define ISM_CHECK_OBJECT(o) (true) // check not required

	// object handle
	template <> class Handle<Object> : public Ref<Object>
	{
		ISM_HANDLE_DEFAULT(Object, ISM_CHECK_OBJECT);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD OBJ object_or_cast(T && o);

	NODISCARD inline OBJ object_or_cast(Object * o) { return OBJ{ o }; }

	NODISCARD inline OBJ object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline TYPE typeof_generic(std::type_info const & t); // cast.hpp

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		if constexpr (is_ref_v<T>)
		{
			return typeof<typename T::value_type>();
		}
		else
		{
			return T::get_class();
		}
	}

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept
	{
		return typeof_generic(typeid(T));
	}

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return o ? o->get_type() : nullptr;
	}

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept
	{
		return typeof(FWD_OBJ(o));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline bool isinstance_generic(OBJ const & o, std::type_info const & t); // cast.hpp

	template <class T, class O = OBJ, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		TYPE const a{ typeof(FWD(o)) }, b{ typeof<T>() };

		return a && b && (a.is(b) || a.is_subtype(b));
	}

	template <class T, class O = OBJ, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		return isinstance_generic(FWD(o), typeid(T));
	}

	template <class A = OBJ, class B = OBJ
	> NODISCARD bool isinstance(A const & lhs, B const & rhs)
	{
		TYPE const a{ typeof(lhs) }, b{ TYPE::check_(rhs) ? rhs : typeof(rhs) };

		return a && b && (a.is(b) || a.is_subtype(b));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJ
	> NODISCARD OBJ getattr(OBJ o, Index && i)
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

	template <class Index = OBJ, class Value = OBJ
	> NODISCARD OBJ getattr(OBJ o, Index && i, Value && defval)
	{
		if (STR str_name{ FWD_OBJ(i) }; str_name && hasattr(o, str_name))
		{
			return getattr(o, str_name);
		}
		else
		{
			return FWD_OBJ(defval);
		}
	}

	template <class Index = OBJ, class Value = OBJ
	> Error setattr(OBJ o, Index && i, Value && v)
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

	template <class Index = OBJ
	> NODISCARD bool hasattr(OBJ o, Index && i)
	{
		STR str_name{ FWD_OBJ(i) };

		if (TYPE type; !str_name || !o || !(type = typeof(o)))
		{
			return false;
		}
		else if (type->tp_getattro == generic_getattr)
		{
			return generic_getattr_with_dict(o, str_name, nullptr).is_valid();
		}
		else if (type->tp_getattro)
		{
			return (type->tp_getattro)(o, str_name).is_valid();
		}
		else if (type->tp_getattr)
		{
			return (type->tp_getattr)(o, str_name.c_str()).is_valid();
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Index = OBJ
	> NODISCARD OBJ getitem(OBJ o, Index && i)
	{
		if (!o.is_valid()) { return nullptr; }
		
		else if constexpr (mpl::is_string_v<Index>) { return CHECK(DICT(o))[FWD(i)]; }
		
		else if constexpr (std::is_integral_v<Index>) { return CHECK(LIST(o))[FWD(i)]; }
		
		else if (DICT::check_(o)) { return DICT(o)[FWD(i)]; }
		
		else if (LIST::check_(o)) { return LIST(o)[FWD(i)]; }

		else if (OBJ getter{ getattr(o, "__getitem__") }) { return getter(o, FWD_OBJ(i)); }
		
		else { return nullptr; }
	}

	template <class Index = OBJ, class Value = OBJ
	> Error setitem(OBJ o, Index && i, Value && v)
	{
		if (!o.is_valid()) { return Error_Unknown; }
		
		else if constexpr (mpl::is_string_v<Index>) { return (CHECK(DICT(o))[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if constexpr (std::is_integral_v<Index>) { return (CHECK(LIST(o))[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if (DICT::check_(o)) { return (DICT(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if (LIST::check_(o)) { return (LIST(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }

		else if (OBJ setter{ getattr(o, "__setitem__") }) { return setter(o, FWD_OBJ(i), FWD_OBJ(v)), Error_None; }
		
		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) object_alloc(TYPE type);

	ISM_API_FUNC(Error) object_init(OBJ self, OBJ args);

	ISM_API_FUNC(OBJ) object_new(TYPE type, OBJ args);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict);

	inline OBJ generic_getattr(OBJ obj, OBJ name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJ obj, OBJ name, OBJ value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
