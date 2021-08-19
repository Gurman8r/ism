#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define ISM_OBJECT_COMMON(m_class, m_inherits)										\
private:																			\
	friend class ism::TypeDB;														\
																					\
	friend class ism::Handle<m_class>;												\
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object default
#define ISM_OBJECT(m_class, m_inherits)												\
ISM_OBJECT_COMMON(m_class, m_inherits)												\
																					\
protected:																			\
	FORCE_INLINE virtual ism::TYPE _get_typev() const override						\
	{																				\
		return m_class::get_class();												\
	}																				\
																					\
public:																				\
	explicit m_class(ism::TYPE const & type) noexcept : m_inherits{ type } {}		\
																					\
	FORCE_INLINE static ism::TYPE get_class() noexcept								\
	{																				\
		return &m_class::_class_type_static;										\
	}																				\
																					\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// implement class type
#define ISM_CLASS_IMPLEMENTATION(m_class, m_var, ...) \
	DECLEXPR(m_class::_class_type_static) = COMPOSE(ism::TypeObject, m_var, ##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object object
	class ISM_API Object : public ObjectAPI<Object>
	{
		friend class TypeDB;

		friend class Handle<Object>;

		static TypeObject _class_type_static;

		InstanceID m_instance_id{};

		RefCount m_refcount{}, m_refcount_init{};

		mutable Ref<TypeObject> m_type{ /* TYPE doesn't exist yet */ };

	protected:
		static void initialize_class(OBJ scope);

		virtual void _initialize_classv(OBJ scope);

		static void _bind_class(OBJ scope);

		FORCE_INLINE static constexpr void (*_get_bind_class())(OBJ) { return &Object::_bind_class; }

		FORCE_INLINE virtual TYPE _get_typev() const;

		void _construct_object();

		explicit Object(TYPE const & type) noexcept;

		virtual void on_inc_ref() {}

		virtual void on_dec_ref() {}

	public:
		virtual ~Object();

		NODISCARD InstanceID get_instance_id() const noexcept { return m_instance_id; }

		NODISCARD int32_t get_ref_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		bool init_ref();

		bool inc_ref(); // returns false if refcount is at zero and didn't get increased
		
		bool dec_ref();

		NODISCARD static TYPE get_class() noexcept;

		NODISCARD TYPE get_type() const noexcept;

		bool set_type(TYPE const & value) noexcept;

		template <class T> NODISCARD T cast() const &; // cast.hpp

		template <class T> NODISCARD T cast() &&; // cast.hpp

		NODISCARD Object * ptr() const noexcept { return const_cast<Object *>(this); }
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

	// no check
#define ISM_NO_CHECK(o) (true)

	// default handle
	template <class T> class Handle : public Ref<T>
	{
		ISM_HANDLE(T, ISM_NO_CHECK);
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
	> NODISCARD bool isinstance(O && obj) noexcept
	{
		return isinstance(FWD_OBJ(obj), typeof<T>());
	}

	template <class T, class O = OBJ, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		return isinstance_generic(FWD(o), typeid(T));
	}

	template <class A = OBJ, class B = OBJ
	> NODISCARD bool isinstance(A const & obj, B const & type)
	{
		return typeof(obj).is_subtype(TYPE::check_(type) ? type : typeof(type));
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
		
		else if constexpr (mpl::is_string_v<Index>) { return DICT(o)[FWD(i)]; }
		
		else if constexpr (std::is_integral_v<Index>) { return LIST(o)[FWD(i)]; }
		
		else if (DICT::check_(o)) { return DICT(o)[FWD(i)]; }
		
		else if (LIST::check_(o)) { return LIST(o)[FWD(i)]; }

		else { return nullptr; }
	}

	template <class Index = OBJ, class Value = OBJ
	> Error setitem(OBJ o, Index && i, Value && v)
	{
		if (!o.is_valid()) { return Error_Unknown; }
		
		else if constexpr (mpl::is_string_v<Index>) { return (DICT(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if constexpr (std::is_integral_v<Index>) { return (LIST(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if (DICT::check_(o)) { return (DICT(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }
		
		else if (LIST::check_(o)) { return (LIST(o)[FWD(i)] = FWD_OBJ(v)), Error_None; }

		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) object_alloc(TYPE type);

	ISM_API_FUNC(OBJ) object_new(TYPE type, OBJ args);

	ISM_API_FUNC(Error) object_init(OBJ self, OBJ args);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict);

	inline OBJ generic_getattr(OBJ obj, OBJ name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJ obj, OBJ name, OBJ value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
