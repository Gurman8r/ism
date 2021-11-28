#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/api/common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define OBJECT_COMMON(m_class, m_inherits)									\
private:																	\
	friend class ism::Internals;											\
																			\
	friend class ism::EmbedClassHelper<m_class>;							\
																			\
	static ism::TypeObject __class_type;									\
																			\
protected:																	\
	static void initialize_class()											\
	{																		\
		if (static bool once{}; !once && (once = true))						\
		{																	\
			ism::get_internals().add_class(&m_class::__class_type);			\
																			\
			if (m_class::__class_type.tp_bind)								\
			{																\
				m_class::__class_type.tp_bind(&m_class::__class_type);		\
			}																\
		};																	\
	}																		\
																			\
	virtual void _initialize_classv() override								\
	{																		\
		m_class::initialize_class();										\
	}																		\
																			\
	FORCE_INLINE virtual ism::TYPE _get_typev() const override				\
	{																		\
		return m_class::get_type_static();									\
	}																		\
																			\
public:																		\
	using base_type = typename m_inherits;									\
																			\
	FORCE_INLINE static ism::TYPE get_type_static() noexcept				\
	{																		\
		return &m_class::__class_type;										\
	}																		\
																			\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// class embedding utility
#define EMBED_CLASS(m_class, m_var, ...)														\
																								\
	/* declare binder */																		\
	template <> class ism::EmbedClassHelper<m_class> final										\
	{																							\
	public:																						\
		static void bind(ism::TypeObject & m_var);												\
	};																							\
																								\
	/* construct type object */																	\
	MEMBER_IMPL(m_class::__class_type) =														\
	COMPOSE_EX(ism::TypeObject, ism::mpl::type_tag<m_class>(), TOSTR(m_class), ##__VA_ARGS__)	\
	+ ism::EmbedClassHelper<m_class>::bind;														\
																								\
	/* implement binder function */																\
	void ism::EmbedClassHelper<m_class>::bind(ism::TypeObject & m_var)							\
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
		friend class OBJ;
		friend class Internals;
		friend class EmbedClassHelper<Object>;
		friend bool predelete_handler(Object *);
		friend void postinitialize_handler(Object *);

		static TypeObject __class_type;

		SafeRefCount m_refcount{}, m_refcount_init{};

		mutable Ref<TypeObject> m_type{};

	protected:
		static void initialize_class();

		virtual void _initialize_classv();

		FORCE_INLINE virtual TYPE _get_typev() const;

		explicit Object() noexcept;

		virtual void _postinitialize() {}

		virtual bool _predelete() { return true; }

		virtual void on_reference() {}

		virtual void on_unreference() {}

	public:
		using base_type = typename void;

		virtual ~Object();

		bool init_ref();

		bool reference();

		bool unreference();

		NODISCARD int32_t get_ref_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		NODISCARD static TYPE get_type_static() noexcept;

		NODISCARD TYPE get_type() const noexcept;

		void set_type(TYPE const & value) noexcept;

		NODISCARD Object * ptr() const noexcept { return const_cast<Object *>(this); }

		template <class T> NODISCARD T cast() const &; // cast.hpp

		template <class T> NODISCARD T cast() &&; // cast.hpp

	public:
		NODISCARD static OBJ generic_getattr(OBJ obj, OBJ name) noexcept;

		NODISCARD static OBJ generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict);

		static Error generic_setattr(OBJ obj, OBJ name, OBJ value) noexcept;

		static Error generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict);
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
			
			else { memdelete(obj); }
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// no check
#define OBJECT_NO_CHECK(o) (true)

	// object ref
	class OBJ : public Ref<Object>
	{
		REF_COMMON(OBJ, OBJECT_NO_CHECK);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IMPLEMENT_DEFAULT_REF_TRAITS(OBJ);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool predelete_handler(Object * value) { return value->_predelete(); }

	inline void postinitialize_handler(Object * value) { value->_postinitialize(); }

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

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept { return T::get_type_static(); }

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept { return VALIDATE(FWD(o))->get_type(); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept { return typeof(FWD_OBJ(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> NODISCARD TYPE baseof() noexcept
	{
		using Base = typename T::base_type;
		if constexpr (std::is_void_v<Base>) { return nullptr; }
		else { return typeof<Base>(); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		if (!obj)
		{
			return FWD_OBJ(defval);
		}
		else if (STR str_name{ FWD_OBJ(index) }; hasattr(obj, str_name))
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
		else if (type->tp_getattro == Object::generic_getattr)
		{
			return Object::generic_getattr(obj, str_name).is_valid();
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
		STR_IDENTIFIER(__getitem__);

		if (!obj) { return nullptr; }

		else if (DICT::check_(obj)) { return ((DICT &)obj)[FWD(index)]; }

		else if (LIST::check_(obj)) { return ((LIST &)obj)[FWD(index)]; }

		else if (OBJ get{ typeof(obj).lookup(&ID___getitem__) }) { return get(obj, FWD_OBJ(index)); }

		else { return nullptr; }
	}

	// setitem
	template <class Index = OBJ, class Value = OBJ
	> Error setitem(OBJ const & obj, Index && index, Value && value)
	{
		STR_IDENTIFIER(__setitem__);

		if (!obj) { return Error_Unknown; }

		else if (DICT::check_(obj)) { return (((DICT &)obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }

		else if (LIST::check_(obj)) { return (((LIST &)obj)[FWD(index)] = FWD_OBJ(value)), Error_None; }

		else if (OBJ set{ typeof(obj).lookup(&ID___setitem__) }) { return set(obj, FWD_OBJ(index), FWD_OBJ(value)), Error_None; }

		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
