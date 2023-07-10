 #ifndef _ISM_BASE_OBJECT_HPP_
#define _ISM_BASE_OBJECT_HPP_

#include <core/object/accessors.hpp>
#include <core/object/ref.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// define class
#define OBJECT_CLASS(m_class, m_inherits)											\
public:																				\
	using base_type = typename m_inherits;											\
																					\
private:																			\
	friend class Ism::Internals;													\
	friend class Ism::_EmbedClassHelper<m_class>;									\
	friend struct Ism::DefaultDelete<m_class>;										\
																					\
	static constexpr Ism::StringView __name_static{ TOSTR(m_class) };				\
																					\
	static Ism::TypeObject __type_static;											\
																					\
protected:																			\
	static void initialize_class()													\
	{																				\
		static ON_SCOPE_ENTER(&)													\
		{																			\
			Ism::internals()->add_class(&m_class::__type_static);					\
																					\
			if (m_class::__type_static.tp_bind) {									\
				ASSERT(m_class::__type_static.tp_bind(&m_class::__type_static));	\
			}																		\
		};																			\
	}																				\
																					\
	virtual void _initialize_classv() override										\
	{																				\
		m_class::initialize_class();												\
	}																				\
																					\
	FORCE_INLINE virtual Ism::StringView _get_class_namev() const noexcept override	\
	{																				\
		return m_class::get_class_name_static();									\
	}																				\
																					\
	FORCE_INLINE virtual Ism::TypeRef _get_typev() const noexcept override			\
	{																				\
		return m_class::get_type_static();											\
	}																				\
																					\
	FORCE_INLINE void (Object::*_get_notification() const)(Notification_)			\
	{																				\
		return (void (Object::*)(Notification_))&m_class::_notification;			\
	}																				\
																					\
	virtual void _notificationv(Notification_ id, bool reversed) override			\
	{																				\
		if (!reversed) {															\
			m_inherits::_notificationv(id, reversed);								\
		}																			\
		if (m_class::_get_notification() != m_inherits::_get_notification()) {		\
			_notification(id);														\
		}																			\
		if (reversed) {																\
			m_inherits::_notificationv(id, reversed);								\
		}																			\
	}																				\
																					\
public:																				\
	FORCE_INLINE static constexpr Ism::StringView get_class_name_static() noexcept	\
	{																				\
		return m_class::__name_static;												\
	}																				\
																					\
	FORCE_INLINE static Ism::TypeRef get_type_static() noexcept						\
	{																				\
		return &m_class::__type_static;												\
	}																				\
																					\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// class embedding utility
#define OBJECT_EMBED(m_class, m_var, ...)											\
																					\
	/* implement embedding helper */												\
	template <> class Ism::_EmbedClassHelper<m_class> final							\
	{																				\
	public:																			\
		static void do_embed(Ism::TypeObject & t);									\
																					\
		static void embed(Ism::TypeObject & t)										\
		{																			\
			/* TODO: can add extra stuff here if needed */							\
																					\
			do_embed(t);															\
		}																			\
	};																				\
																					\
	/* construct the type object using a maker to call the embedder */				\
	Ism::TypeObject m_class::__type_static = MAKER(									\
		Ism::TypeObject,															\
		Ism::mpl::type_tag<m_class>(),												\
		TOSTR(m_class),																\
		##__VA_ARGS__)																\
																					\
	+ Ism::_EmbedClassHelper<m_class>::embed;										\
																					\
	/* implement binder function body */											\
	void Ism::_EmbedClassHelper<m_class>::do_embed(Ism::TypeObject & m_var)			\
																					\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object
namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using Notification_ = typename i32;

	enum : Notification_ {
		Notification_PostInitialize = 1,
		Notification_PreDelete = 2,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base object
	class ISM_API Object : public ObjectAPI<Object>
	{
	public:
		using base_type = typename void;

	private:
		friend class ObjectRef;
		friend class Internals;
		friend class _EmbedClassHelper<Object>;
		friend struct DefaultDelete<Object>;
		friend ISM_API_FUNC(bool) predelete_handler(Object *);
		friend ISM_API_FUNC(void) postinitialize_handler(Object *);

		static constexpr StringView __name_static{ "Object" };

		static TypeObject __type_static;

		SafeRefCount m_refcount{}, m_refcount_init{}; // ref count

		mutable Ref<TypeObject> m_type{}; // type

	protected:
		static void initialize_class();
		virtual void _initialize_classv();

		bool _predelete();
		void _postinitialize();

		void _initialize_object();
		void _finalize_object();

		FORCE_INLINE virtual StringView _get_class_namev() const noexcept { return get_class_name_static(); }
		FORCE_INLINE virtual TypeRef _get_typev() const noexcept;

		virtual void _notificationv(Notification_ id, bool reversed) {}
		void _notification(Notification_ id) {}
		FORCE_INLINE void (Object::*_get_notification() const)(Notification_) { return &Object::_notification; }

		Object() { _initialize_object(); }

	public:
		virtual ~Object() { _finalize_object(); }

		bool init_ref();
		bool inc_ref();
		bool dec_ref();
		NODISCARD i32 get_ref_count() const { return m_refcount.get(); }
		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		void notification(Notification_ id, bool reversed = false);

		NODISCARD static constexpr StringView get_class_name_static() noexcept { return __name_static; }
		NODISCARD StringView get_class_name() const noexcept { return _get_class_namev(); }

		NODISCARD static TypeRef get_type_static() noexcept;
		NODISCARD TypeRef get_type() const noexcept;
		void set_type(TypeRef const & value) noexcept;

		template <class T> NODISCARD T cast() const &;
		template <class T> NODISCARD T cast() &&;

		NODISCARD Object * ptr() const noexcept { return (Object *)this; }

		// generic_getattr
		NODISCARD static ObjectRef generic_getattr_with_dict(ObjectRef obj, ObjectRef name, ObjectRef dict);
		NODISCARD static ObjectRef generic_getattr(ObjectRef obj, ObjectRef name) noexcept;

		// generic_setattr
		NODISCARD static Error_ generic_setattr_with_dict(ObjectRef obj, ObjectRef name, ObjectRef value, ObjectRef dict);
		NODISCARD static Error_ generic_setattr(ObjectRef obj, ObjectRef name, ObjectRef value) noexcept;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(bool) predelete_handler(Object * value);

	ISM_API_FUNC(void) postinitialize_handler(Object * value);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object delete
	template <> struct DefaultDelete<Object>
	{
		template <class U> void operator()(U * ptr) const
		{
			if (Object * obj{ (Object *)ptr }; !obj)
			{
				return;
			}
			else if (obj->m_type && obj->m_type->tp_del)
			{
				obj->m_type->tp_del(obj);
			}
			else
			{
				memdelete(ptr);
			}
		}
	};

	// no check
#define OBJECT_NO_CHECK(o) (true)

	// object ref
	class ObjectRef : public Ref<Object>
	{
		CUSTOM_REF(ObjectRef, OBJECT_NO_CHECK);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct Hasher<ObjectRef> : Hasher<Ref<Object>> {};
	template <> struct EqualTo<ObjectRef> : EqualTo<Ref<Object>> {};
	template <> struct NotEqualTo<ObjectRef> : NotEqualTo<Ref<Object>> {};
	template <> struct Less<ObjectRef> : Less<Ref<Object>> {};
	template <> struct Greater<ObjectRef> : Greater<Ref<Object>> {};
	template <> struct LessEqual<ObjectRef> : LessEqual<Ref<Object>> {};
	template <> struct GreaterEqual<ObjectRef> : GreaterEqual<Ref<Object>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD ObjectRef object_or_cast(T && o) noexcept;

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> NODISCARD inline ObjectRef object_or_cast(T const * o) noexcept { return Ref<T>{ (T *)o }; }

	NODISCARD inline ObjectRef object_or_cast(cstring s) noexcept { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TypeRef typeof() noexcept { return T::get_type_static(); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> NODISCARD TypeRef typeof(T && o) noexcept { return typeof(FWD_OBJ(o)); }

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD TypeRef typeof(T && o) noexcept
	{
		if constexpr (std::is_pointer_v<std::decay_t<decltype(o)>>) {
			return (o != nullptr) ? o->get_type() : nullptr;
		}
		else if constexpr (is_ref_v<std::decay_t<decltype(o)>>) {
			return o.is_valid() ? o->get_type() : nullptr;
		}
		else {
			return o.get_type();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> NODISCARD TypeRef super() noexcept
	{
		if constexpr (std::is_void_v<T::base_type>) {
			return nullptr;
		}
		else {
			return typeof<T::base_type>();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class O = ObjectRef, std::enable_if_t<is_object_api_v<T>, int> = 0
	> NODISCARD bool isinstance(O && obj) noexcept
	{
		return isinstance(FWD_OBJ(obj), typeof<T>());
	}

	template <class A = ObjectRef, class B = ObjectRef
	> NODISCARD bool isinstance(A const & obj, B const & type)
	{
		return typeof(obj).is_subtype(TypeRef::check_(type) ? (TypeRef)type : typeof(type));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getattr
	template <class Index = ObjectRef
	> NODISCARD ObjectRef getattr(ObjectRef const & obj, Index && index)
	{
		if (!obj) { return nullptr; }

		TypeRef type{ typeof(obj) };

		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_getattr) { return type->tp_getattr(obj, FWD(index)); }

			else if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else { return nullptr; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else if (type->tp_getattr) { return type->tp_getattr(obj, StringRef(FWD(index)).c_str()); }

			else { return nullptr; }
		}
	}

	// getattr (default)
	template <class Index = ObjectRef, class Value = ObjectRef
	> NODISCARD ObjectRef getattr(ObjectRef const & obj, Index && index, Value && defval)
	{
		if (!obj)
		{
			return FWD_OBJ(defval);
		}
		else if (StringRef str_name{ FWD_OBJ(index) }; hasattr(obj, str_name))
		{
			return getattr(obj, str_name);
		}
		else
		{
			return FWD_OBJ(defval);
		}
	}

	// setattr
	template <class Index = ObjectRef, class Value = ObjectRef
	> Error_ setattr(ObjectRef const & obj, Index && index, Value && value)
	{
		if (!obj) { return Error_Failed; }

		TypeRef type{ typeof(obj) };
		
		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_setattr) { return type->tp_setattr(obj, FWD(index), FWD_OBJ(value)); }

			else if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else { return Error_Failed; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else if (type->tp_setattr) { return type->tp_setattr(obj, StringRef(FWD(index)).c_str(), FWD_OBJ(value)); }

			else { return Error_Failed; }
		}
	}

	// hasattr
	template <class Index = ObjectRef
	> NODISCARD bool hasattr(ObjectRef const & obj, Index && index)
	{
		if (!obj) { return false; }

		TypeRef type{ typeof(obj) };

		if (StringRef str_name{ FWD_OBJ(index) }; !str_name)
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
	template <class Index = ObjectRef
	> NODISCARD ObjectRef getitem(ObjectRef const & obj, Index && index)
	{
		STRING_IDENTIFIER(__getitem__);

		if (!obj) { return nullptr; }

		else if (DictRef::check_(obj)) { return ((DictRef &)obj)[FWD(index)]; }

		else if (ListRef::check_(obj)) { return ((ListRef &)obj)[FWD(index)]; }

		else if (ObjectRef get{ typeof(obj).lookup(&ID___getitem__) }) { return get(obj, FWD_OBJ(index)); }

		else { return nullptr; }
	}

	// setitem
	template <class Index = ObjectRef, class Value = ObjectRef
	> Error_ setitem(ObjectRef const & obj, Index && index, Value && value)
	{
		STRING_IDENTIFIER(__setitem__);

		if (!obj) { return Error_Failed; }

		else if (DictRef::check_(obj)) { return (((DictRef &)obj)[FWD(index)] = FWD_OBJ(value)), Error_OK; }

		else if (ListRef::check_(obj)) { return (((ListRef &)obj)[FWD(index)] = FWD_OBJ(value)), Error_OK; }

		else if (ObjectRef set{ typeof(obj).lookup(&ID___setitem__) }) { return set(obj, FWD_OBJ(index), FWD_OBJ(value)), Error_OK; }

		else { return Error_Failed; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BASE_OBJECT_HPP_
