#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/object/detail/common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define OBJECT_CLASS(m_class, m_inherits)						\
private:														\
	friend class ism::Internals;								\
																\
	friend class ism::Handle<m_class>;							\
																\
	static ism::TypeObject ob_class;							\
																\
protected:														\
	static void initialize_class()								\
	{															\
		if (static bool once{}; !once && (once = true))			\
		{														\
			ism::get_internals().add_class(&m_class::ob_class);	\
																\
			if (m_class::ob_class.tp_bind)						\
			{													\
				m_class::ob_class.tp_bind(&m_class::ob_class);	\
			}													\
		};														\
	}															\
																\
	virtual void _initialize_classv() override					\
	{															\
		m_class::initialize_class();							\
	}															\
																\
	FORCE_INLINE virtual ism::TYPE _get_typev() const override	\
	{															\
		return m_class::get_class();							\
	}															\
																\
public:															\
	using base_type = typename m_inherits;						\
																\
	FORCE_INLINE static ism::TYPE get_class() noexcept			\
	{															\
		return &m_class::ob_class;								\
	}															\
																\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// "_obj_imp_class_"
#define __OBJ_IMP__(m_class) \
	CAT(CAT(_obj_imp_, m_class), _)

// implement object class
#define OBJECT_IMP(m_class, m_var, ...)															\
																								\
	/* declare binder function */																\
	namespace ism { static void __OBJ_IMP__(m_class)(ism::TypeObject & m_var); }				\
																								\
	/* construct type */																		\
	MEMBER_IMP(m_class::ob_class) =																\
	COMPOSE_EX(ism::TypeObject, ism::mpl::type_tag<m_class>(), TOSTR(m_class), ##__VA_ARGS__)	\
	+ ism::__OBJ_IMP__(m_class);																\
																								\
	/* implement binder function */																\
	void ism::__OBJ_IMP__(m_class)(ism::TypeObject & m_var)										\
																								\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object object
	class ISM_API Object : public ObjectAPI<Object>
	{
	private:
		friend class Internals;

		friend class Handle<Object>;

		static TypeObject ob_class;

	protected:
		mutable Ref<TypeObject> ob_type{};

		RefCount m_refcount{}, m_refcount_init{};

	protected:
		static void initialize_class();

		virtual void _initialize_classv();

		FORCE_INLINE virtual TYPE _get_typev() const;

		void _construct_object();

		explicit Object() noexcept;

		virtual void on_inc_ref() {}

		virtual void on_dec_ref() {}

	public:
		using base_type = typename void;

		virtual ~Object();

		NODISCARD Object * ptr() const noexcept { return const_cast<Object *>(this); }

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object delete
	template <> struct DefaultDelete<Object>
	{
		template <class U> void operator()(U * ptr) const
		{
			if (!ptr) { return; }

			Object * obj{ (Object *)ptr };

			TYPE type{ typeof(obj) };

			if (type && type->tp_del) { type->tp_del(obj); }
			
			else { memdelete((Object *)ptr); }
		}
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

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> NODISCARD inline OBJ object_or_cast(T const * o) { return Ref<T>{ (T *)o }; }

	NODISCARD inline OBJ object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline TYPE typeof_generic(std::type_info const & t); // cast.hpp

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept { return typeof_generic(typeid(T)); }

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept { return T::get_class(); }

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept { return CHECK(FWD(o))->get_type(); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept { return typeof(FWD_OBJ(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE baseof() noexcept
	{
		using Base = typename T::base_type;
		if constexpr (std::is_void_v<Base>) { return nullptr; }
		else { return typeof<Base>(); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline bool isinstance_generic(OBJ const & o, std::type_info const & t); // cast.hpp

	template <class T, class O = OBJ, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && o) noexcept
	{
		return isinstance_generic(FWD(o), typeid(T));
	}

	template <class T, class O = OBJ, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && obj) noexcept
	{
		return isinstance(FWD_OBJ(obj), typeof<T>());
	}

	template <class A = OBJ, class B = OBJ
	> NODISCARD bool isinstance(A const & obj, B const & type)
	{
		return typeof(obj).is_subtype(TYPE::check_(type) ? type : typeof(type));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getattr
	template <class Index = OBJ
	> NODISCARD OBJ getattr(OBJ const & obj, Index && index)
	{
		if (!obj) { return nullptr; }

		TYPE type{ typeof(obj) };

		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_getattr) { return type->tp_getattr(obj, FWD(index)); }

			else if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else { return nullptr; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else if (type->tp_getattr) { return type->tp_getattr(obj, STR(FWD(index)).c_str()); }

			else { return nullptr; }
		}
	}

	// getattr (default)
	template <class Index = OBJ, class Value = OBJ
	> NODISCARD OBJ getattr(OBJ const & obj, Index && index, Value && defval)
	{
		if (STR str_name{ FWD_OBJ(index) }; str_name && hasattr(obj, str_name))
		{
			return getattr(obj, str_name);
		}
		else
		{
			return FWD_OBJ(defval);
		}
	}

	// setattr
	template <class Index = OBJ, class Value = OBJ
	> Error setattr(OBJ const & obj, Index && index, Value && value)
	{
		if (!obj) { return Error_Unknown; }

		TYPE type{ typeof(obj) };
		
		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_setattr) { return type->tp_setattr(obj, FWD(index), FWD_OBJ(value)); }

			else if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else { return Error_Unknown; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else if (type->tp_setattr) { return type->tp_setattr(obj, STR(FWD(index)).c_str(), FWD_OBJ(value)); }

			else { return Error_Unknown; }
		}
	}

	// hasattr
	template <class Index = OBJ
	> NODISCARD bool hasattr(OBJ const & obj, Index && index)
	{
		if (!obj) { return false; }

		TYPE type{ typeof(obj) };

		if (STR str_name{ FWD_OBJ(index) }; !str_name)
		{
			return false;
		}
		else if (type->tp_getattro == generic_getattr)
		{
			return generic_getattr(obj, str_name).is_valid();
		}
		else if (type->tp_getattro)
		{
			return (type->tp_getattro)(obj, str_name).is_valid();
		}
		else if (type->tp_getattr)
		{
			return (type->tp_getattr)(obj, str_name.c_str()).is_valid();
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getitem
	template <class Index = OBJ
	> NODISCARD OBJ getitem(OBJ const & obj, Index && index)
	{
		if (!obj) { return nullptr; }
		
		else if constexpr (mpl::is_string_v<Index>) { return DICT(obj)[FWD(index)]; }
		
		else if constexpr (std::is_integral_v<Index>) { return LIST(obj)[FWD(index)]; }
		
		else if (DICT::check_(obj)) { return DICT(obj)[FWD(index)]; }
		
		else if (LIST::check_(obj)) { return LIST(obj)[FWD(index)]; }

		else { return nullptr; }
	}

	// setitem
	template <class Index = OBJ, class Value = OBJ
	> Error setitem(OBJ const & obj, Index && index, Value && value)
	{
		if (!obj) { return Error_Unknown; }
		
		else if constexpr (mpl::is_string_v<Index>) { return (DICT(obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }
		
		else if constexpr (std::is_integral_v<Index>) { return (LIST(obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }
		
		else if (DICT::check_(obj)) { return (DICT(obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }
		
		else if (LIST::check_(obj)) { return (LIST(obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }

		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict);

	NODISCARD inline OBJ generic_getattr(OBJ obj, OBJ name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJ obj, OBJ name, OBJ value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
