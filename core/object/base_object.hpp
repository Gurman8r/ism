#ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/object/detail/accessors.hpp>
#include <core/object/detail/ref.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object common
#define OBJECT_COMMON(m_class, m_inherits)											\
private:																			\
	friend class ism::Internals;													\
																					\
	friend class ism::EmbedObjectClassHelper<m_class>;								\
																					\
	friend struct ism::DefaultDelete<m_class>;										\
																					\
	static constexpr ism::StringView g_name_static{ TOSTR(m_class) };				\
																					\
	static ism::TypeObject g_type_static;											\
																					\
protected:																			\
	static void initialize_class()													\
	{																				\
		static SCOPE_ENTER(&)														\
		{																			\
			INTERNALS->add_class(&m_class::g_type_static);							\
																					\
			if (m_class::g_type_static.tp_install)									\
			{																		\
				ASSERT(m_class::g_type_static.tp_install(&m_class::g_type_static));	\
			}																		\
		};																			\
	}																				\
																					\
	virtual void _initialize_classv() override										\
	{																				\
		m_class::initialize_class();												\
	}																				\
																					\
	FORCE_INLINE virtual ism::StringView _get_classv() const noexcept override		\
	{																				\
		return m_class::get_class_static();											\
	}																				\
																					\
	FORCE_INLINE virtual ism::TYPE _get_typev() const noexcept override				\
	{																				\
		return m_class::get_type_static();											\
	}																				\
																					\
public:																				\
	using base_type = typename m_inherits;											\
																					\
	FORCE_INLINE static constexpr ism::StringView get_class_static() noexcept		\
	{																				\
		return m_class::g_name_static;												\
	}																				\
																					\
	FORCE_INLINE static ism::TYPE get_type_static() noexcept						\
	{																				\
		return &m_class::g_type_static;												\
	}																				\
																					\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// class embedding utility
#define OBJECT_EMBED(m_class, m_var, ...)													\
																							\
	/* declare binder */																	\
	template <> class ism::EmbedObjectClassHelper<m_class> final							\
	{																						\
	public: static void embed(ism::TypeObject &);											\
	};																						\
																							\
	/* construct type object */																\
	MEMBER_IMPL(m_class::g_type_static) =													\
																							\
	MAKE_EX(ism::TypeObject, ism::mpl::type_tag<m_class>(), TOSTR(m_class), ##__VA_ARGS__)	\
																							\
	+ ism::EmbedObjectClassHelper<m_class>::embed;											\
																							\
	/* implement binder function body */													\
	void ism::EmbedObjectClassHelper<m_class>::embed(ism::TypeObject & m_var)				\
																							\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base object
	class ISM_API Object : public ObjectAPI<Object>
	{
	private:
		friend class OBJ;
		friend class Internals;
		friend class EmbedObjectClassHelper<Object>;
		friend struct DefaultDelete<Object>;
		friend bool predelete_handler(Object *);
		friend void postinitialize_handler(Object *);

		static constexpr StringView g_name_static{ "Object" };

		static TypeObject g_type_static;

		SafeRefCount m_refcount{}, m_refcount_init{};

		mutable Ref<TypeObject> m_type{};

	protected:
		static void initialize_class();

		virtual void _initialize_classv();

		FORCE_INLINE virtual StringView _get_classv() const noexcept { return get_class_static(); }

		FORCE_INLINE virtual TYPE _get_typev() const noexcept;

		Object() noexcept;

		virtual void _postinitialize();

		virtual bool _predelete();

		virtual void _reference();

		virtual void _unreference();

	public:
		using base_type = typename void;

		virtual ~Object();

		bool init_ref();

		bool reference();

		bool unreference();

		int32_t get_ref_count() const { return m_refcount.get(); }

		bool has_references() const { return m_refcount_init.get() != 1; }

	public:
		static constexpr StringView get_class_static() noexcept { return g_name_static; }

		static TYPE get_type_static() noexcept;

		StringView get_class() const noexcept { return _get_classv(); }

		TYPE get_type() const noexcept;

		void set_type(TYPE const & value) noexcept;

		template <class T> T cast() const &; // cast.hpp

		template <class T> T cast() &&; // cast.hpp

		Object * ptr() const noexcept { return const_cast<Object *>(this); } // :^]

	public:
		static OBJ generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict);

		static OBJ generic_getattr(OBJ obj, OBJ name);

		static Error_ generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict);

		static Error_ generic_setattr(OBJ obj, OBJ name, OBJ value);
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
	> auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> OBJ object_or_cast(T && o);

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> inline OBJ object_or_cast(T const * o) { return Ref<T>{ (T *)o }; }

	inline OBJ object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> TYPE typeof() noexcept { return T::get_type_static(); }

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> TYPE typeof(T && o) noexcept { return VALIDATE(FWD(o))->get_type(); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> TYPE typeof(T && o) noexcept { return typeof(FWD_OBJ(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> TYPE baseof() noexcept
	{
		if constexpr (std::is_void_v<T::base_type>) { return nullptr; }

		else { return typeof<T::base_type>(); }
	}

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> constexpr StringView nameof() noexcept
	{
		return T::get_class_static();
	}

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> Object * pointerof(T const & o) noexcept
	{
		if constexpr (std::is_pointer_v<std::decay_t<decltype(o)>>)
		{
			return (Object *)o;
		}
		else
		{
			return o.ptr();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class O = OBJ, std::enable_if_t<is_api_v<T>, int> = 0
	> bool isinstance(O && obj) noexcept
	{
		return isinstance(FWD_OBJ(obj), typeof<T>());
	}

	template <class A = OBJ, class B = OBJ
	> bool isinstance(A const & obj, B const & type)
	{
		return typeof(obj).is_subtype(TYPE::check_(type) ? (TYPE)type : typeof(type));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getattr
	template <class Index = OBJ
	> OBJ getattr(OBJ const & obj, Index && index)
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
	> OBJ getattr(OBJ const & obj, Index && index, Value && defval)
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
	> Error_ setattr(OBJ const & obj, Index && index, Value && value)
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
	> bool hasattr(OBJ const & obj, Index && index)
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
	> OBJ getitem(OBJ const & obj, Index && index)
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
	> Error_ setitem(OBJ const & obj, Index && index, Value && value)
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
