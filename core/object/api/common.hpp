#ifndef _ISM_COMMON_HPP_
#define _ISM_COMMON_HPP_

#include <core/typedefs.hpp>
#include <core/string/path.hpp>
#include <core/math/color.hpp>
#include <core/math/quat.hpp>
#include <core/math/transform.hpp>
#include <core/math/transform_2d.hpp>
#include <core/math/vector4.hpp>
#include <core/math/rect.hpp>
#include <core/templates/any.hpp>
#include <core/templates/atomic.hpp>
#include <core/templates/duration.hpp>
#include <core/templates/ecs.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>

#define FWD_OBJ(expr) \
	(ism::api::object_or_cast(FWD(expr)))

#define STR_IDENTIFIER(m_name) \
	static ism::StringObject CAT(ID_, m_name) { TOSTR(m_name) }

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace api
	{
		class Internals;

		template <class T> class EmbedClassHelper;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _API_Tag { /* used to determine if type derives ObjectAPI<> */ };
	
	template <class T
	> constexpr bool is_api_v{ std::is_base_of_v<_API_Tag, mpl::intrinsic_t<T>> };

	template <class Derived> class ObjectAPI;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Object;
	class TypeObject;
	class IntObject;
	class FloatObject;
	class StringObject;
	class ListObject;
	class DictObject;
	class CapsuleObject;
	class FunctionObject;
	class MethodObject;
	class PropertyObject;
	class CppFunctionObject;
	class ModuleObject;

	template <class T
	> constexpr bool is_base_object_v{ std::is_base_of_v<Object, mpl::intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _Ref_Tag { /* used to determine if type derives Ref<> */ };

	template <class T
	> constexpr bool is_ref_v{ std::is_base_of_v<_Ref_Tag, mpl::intrinsic_t<T>> };

	template <class T> class Ref;

	class OBJ;
	class TYPE;
	class INT;
	class FLT;
	class STR;
	class LIST;
	class DICT;
	class CAPSULE;
	class FUNCTION;
	class METHOD;
	class PROPERTY;
	class CPP_FUNCTION;
	class MODULE;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy> class Accessor;
	namespace accessor_policies {
		template <class I> struct AttrPolicy;
		template <class I> struct ItemPolicy;
	}
	template <class I> ALIAS(AttrAccessor) Accessor<accessor_policies::AttrPolicy<I>>;
	template <class I> ALIAS(ItemAccessor) Accessor<accessor_policies::ItemPolicy<I>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MAKE_ENUM(DataType)
	{
		DataType_Invalid = -1,
		DataType_Int8, DataType_Int16, DataType_Int32, DataType_Int64,
		DataType_Uint8, DataType_Uint16, DataType_Uint32, DataType_Uint64,
		DataType_Float, DataType_Double,
		DataType_String, DataType_Object,
		DataType_Char, DataType_Byte, DataType_Bool,
		DataType_None,

#if ARCHITECTURE < 64
		DataType_SizeT = DataType_Uint32,
		DataType_SSizeT = DataType_Int32,
#else
		DataType_SizeT = DataType_Uint64,
		DataType_SSizeT = DataType_Int64,
#endif
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MAKE_ENUM(ReturnValuePolicy)
	{
		ReturnValuePolicy_Automatic,
		ReturnValuePolicy_AutomaticReference,
		ReturnValuePolicy_TakeOwnership,
		ReturnValuePolicy_Copy,
		ReturnValuePolicy_Move,
		ReturnValuePolicy_Reference,
		ReturnValuePolicy_ReferenceInternal,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MAKE_ENUM(TypeFlags)
	{
		TypeFlags_None,

		TypeFlags_HeapType			= 1 << 0,
		TypeFlags_BaseType			= 1 << 1,
		TypeFlags_HaveVectorCall	= 1 << 2,
		TypeFlags_Ready				= 1 << 3,
		TypeFlags_Readying			= 1 << 4,
		TypeFlags_MethodDescriptor	= 1 << 5,
		TypeFlags_IsAbstract		= 1 << 6,
		TypeFlags_IsFinal			= 1 << 7,

		TypeFlags_Int_Subclass		= 1 << 25,
		TypeFlags_Float_Subclass	= 1 << 26,
		TypeFlags_Str_Subclass		= 1 << 27,
		TypeFlags_List_Subclass		= 1 << 28,
		TypeFlags_Dict_Subclass		= 1 << 29,
		TypeFlags_Type_Subclass		= 1 << 30,

		TypeFlags_Default = TypeFlags_None,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		OBJ(*)(OBJ a);
	ALIAS(binaryfunc)		OBJ(*)(OBJ a, OBJ b);
	ALIAS(ternaryfunc)		OBJ(*)(OBJ a, OBJ b, OBJ c);
	
	ALIAS(inquiry)			bool(*)(OBJ obj);
	ALIAS(sizeargfunc)		OBJ(*)(OBJ obj, ssize_t i);
	ALIAS(sizesizeargfunc)	OBJ(*)(OBJ obj, ssize_t i, ssize_t j);
	ALIAS(objobjproc)		int32_t(*)(OBJ lhs, OBJ rhs);

	ALIAS(getattrfunc)		OBJ(*)(OBJ obj, cstring name);
	ALIAS(setattrfunc)		Error(*)(OBJ obj, cstring name, OBJ value);
	ALIAS(getattrofunc)		OBJ(*)(OBJ obj, OBJ name);
	ALIAS(setattrofunc)		Error(*)(OBJ obj, OBJ name, OBJ value);
	ALIAS(descrgetfunc)		OBJ(*)(OBJ descr, OBJ obj, OBJ cls);
	ALIAS(descrsetfunc)		Error(*)(OBJ descr, OBJ obj, OBJ value);
	
	ALIAS(bindfunc)			TYPE(*)(TYPE type);
	ALIAS(newfunc)			OBJ(*)(TYPE type, OBJ args);
	ALIAS(delfunc)			void(*)(Object * ptr);
	ALIAS(cmpfunc)			int32_t(*)(OBJ lhs, OBJ rhs);
	ALIAS(hashfunc)			hash_t(*)(OBJ obj);
	ALIAS(lenfunc)			ssize_t(*)(OBJ obj);
	ALIAS(reprfunc)			STR(*)(OBJ obj);
	ALIAS(vectorcallfunc)	OBJ(*)(OBJ self, OBJ const * argc, size_t argv);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object_api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : public _API_Tag
	{
		NODISCARD auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		template <class Index = cstring
		> NODISCARD auto attr(Index && i) const { return AttrAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Index = cstring
		> NODISCARD auto operator[](Index && i) const { return ItemAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Value = OBJ
		> NODISCARD bool contains(Value && value) const { return attr("__contains__")(FWD(value)).cast<bool>(); }

		template <ReturnValuePolicy_ policy = ReturnValuePolicy_AutomaticReference, class ... Args
		> OBJ operator()(Args && ... args) const; // call.hpp

	public:
		NODISCARD bool is(ObjectAPI const & other) const noexcept { return derived().ptr() == other.derived().ptr(); }

		template <class O, class = std::enable_if_t<is_base_object_v<O>>
		> NODISCARD bool is(O const * other) const noexcept { return derived().ptr() == other; }

		template <class O, class = std::enable_if_t<is_api_v<O>>
		> NODISCARD bool is(O const & other) const noexcept { return derived().ptr() == other.ptr(); }
		
	public:
		NODISCARD bool equal_to(ObjectAPI const & other) const noexcept { return compare(other) == 0; }
		
		NODISCARD bool not_equal_to(ObjectAPI const & other) const noexcept { return compare(other) != 0; }
		
		NODISCARD bool less(ObjectAPI const & other) const noexcept { return compare(other) < 0; }
		
		NODISCARD bool less_equal(ObjectAPI const & other) const noexcept { return compare(other) <= 0; }
		
		NODISCARD bool greater(ObjectAPI const & other) const noexcept { return compare(other) > 0; }
		
		NODISCARD bool greater_equal(ObjectAPI const & other) const noexcept { return compare(other) >= 0; }

	private:
		NODISCARD auto compare(ObjectAPI const & o) const
		{
			if (auto self{ derived().ptr() }, other{ o.derived().ptr() }; self == other)
			{
				return 0;
			}
			else if (cmpfunc cmp{ api::typeof(self)->tp_cmp })
			{
				return cmp(self, other);
			}
			else
			{
				return util::compare((void *)self, (void *)other);
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

	template <class T> struct Hasher<ObjectAPI<T>>
	{
		template <class U> hash_t operator()(U const & o) const { return api::hash(o); }
	};

	template <class T> struct EqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ref
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Type
	> class NOVTABLE Ref : public _Ref_Tag, public ObjectAPI<Ref<_Type>>
	{
	public:
		using value_type = typename _Type;

	protected:
		value_type * m_ptr{};

		void ref(Ref const & value)
		{
			if (value.m_ptr == m_ptr) { return; }
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) { m_ptr->reference(); }
		}

		void ref_pointer(value_type * value)
		{
			VERIFY("INVALID POINTER" && value);

			if (value->init_ref()) { m_ptr = value; }
		}

	public:
		~Ref() noexcept { unref(); }

		Ref() noexcept = default;

		Ref(nullptr_t) noexcept {}

		Ref(value_type * value) { if (value) { ref_pointer(value); } }

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(value_type && value) noexcept { instance(std::move(value)); }

		Ref & operator=(nullptr_t) { return unref(), (*this); }

		Ref & operator=(Ref const & value) { return reset(value), (*this); }

		template <class U
		> Ref & operator=(Ref<U> const & value) { return reset(value), (*this); }

		Ref & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }

	public:
		template <class ... Args
		> static auto new_(Args && ... args) { return Ref<value_type>{ value_type{ FWD(args)... } }; }

		template <class U> NODISCARD U cast() const &; // cast.hpp

		template <class U> NODISCARD U cast() &&; // cast.hpp

		NODISCARD static auto get_type_static() noexcept { return value_type::get_type_static(); }

	public:
		template <class ... Args
		> void instance(Args && ... args)
		{
			ref(ism::construct_or_initialize<value_type>(FWD(args)...));
		}

		void reset(Ref const & value)
		{
			ref(value);
		}

		template <class U
		> void reset(U * value)
		{
			if (m_ptr == value) { return; }
			unref();
			value_type * r{ dynamic_cast<value_type *>(value) };
			if (r) { ref_pointer(r); }
		}

		template <class U
		> void reset(Ref<U> const & value)
		{
			Object * other{ static_cast<Object *>(value.ptr()) };
			if (!other) { unref(); return; }
			Ref r;
			r.m_ptr = dynamic_cast<value_type *>(other);
			ref(r);
			r.m_ptr = nullptr;
		}

		void unref()
		{
			if (m_ptr && m_ptr->unreference()) { ism::call_default_delete(m_ptr); }
			
			m_ptr = nullptr;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }
		NODISCARD bool is_null() const noexcept { return m_ptr == nullptr; }
		NODISCARD bool is_valid() const noexcept { return m_ptr != nullptr; }

		NODISCARD auto ptr() noexcept { return m_ptr; }
		NODISCARD auto ptr() const noexcept { return m_ptr; }

		NODISCARD auto operator*() noexcept { return m_ptr; }
		NODISCARD auto operator*() const noexcept { return m_ptr; }

		NODISCARD auto operator->() noexcept { return m_ptr; }
		NODISCARD auto operator->() const noexcept { return m_ptr; }

		NODISCARD bool operator==(value_type const * value) const noexcept { return (m_ptr == value) || ((m_ptr && value) && m_ptr->equal_to(*value)); }
		NODISCARD bool operator!=(value_type const * value) const noexcept { return (m_ptr != value) && ((m_ptr && value) && m_ptr->not_equal_to(*value)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> bool operator==(T const * lhs, Ref<T> const & rhs) noexcept { return rhs.operator==(lhs); }

	template <class T> bool operator!=(T const * lhs, Ref<T> const & rhs) noexcept { return rhs.operator!=(lhs); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher<Ref<T>> : Hasher<ObjectAPI<Ref<T>>> {};
	template <class T> struct EqualTo<Ref<T>> : EqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct NotEqualTo<Ref<T>> : NotEqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct Less<Ref<T>> : Less<ObjectAPI<Ref<T>>> {};
	template <class T> struct Greater<Ref<T>> : Greater<ObjectAPI<Ref<T>>> {};
	template <class T> struct LessEqual<Ref<T>> : LessEqual<ObjectAPI<Ref<T>>> {};
	template <class T> struct GreaterEqual<Ref<T>> : GreaterEqual<ObjectAPI<Ref<T>>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ref common
#define REF_COMMON(m_class, m_check)																	\
public:																									\
	ALIAS(base_type) ism::Ref<value_type>;																\
																										\
	NODISCARD static bool check_(ism::Ref<Object> const & o) { return o && (bool)(m_check(o)); }		\
																										\
	NODISCARD bool check() const { return m_ptr && (bool)(m_check(m_ptr)); }							\
																										\
	~m_class() noexcept = default;																		\
																										\
	m_class() noexcept = default;																		\
																										\
	m_class(nullptr_t) noexcept {}																		\
																										\
	m_class(value_type * value) { if (value) { ref_pointer(value); } }									\
																										\
	m_class(base_type const & value) { ref(value); }													\
																										\
	template <class U> m_class(ism::Ref<U> const & value) { reset(value); }								\
																										\
	m_class(value_type && value) noexcept { instance(std::move(value)); }								\
																										\
	m_class & operator=(nullptr_t) { return unref(), (*this); }											\
																										\
	m_class & operator=(base_type const & value) { return reset(value), (*this); }						\
																										\
	template <class U> m_class & operator=(ism::Ref<U> const & value) { return reset(value), (*this); }	\
																										\
	m_class & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }	\
																										\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// accessors
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// accessor
	template <class Policy
	> class Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using obj_type = typename Ref<Object>;
		using key_type = typename Policy::key_type;

		Accessor(obj_type const & obj, key_type const & key) : m_obj{ obj }, m_key{ key } {}

		Accessor(Accessor const &) = default;

		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(obj_type(a)); }

		void operator=(Accessor const & a) & { operator=(obj_type(a)); }

		template <class Value> decltype(auto) operator=(Value && value) &&
		{
			return Policy::set(m_obj, m_key, FWD_OBJ(value)), (*this);
		}

		template <class Value> decltype(auto) operator=(Value && value) &
		{
			return (get_cache() = FWD_OBJ(value)), (*this);
		}

		NODISCARD auto ptr() const { return const_cast<Object *>(get_cache().ptr()); }

		template <class T> NODISCARD operator Ref<T>() const { return get_cache(); }

		template <class T> NODISCARD auto cast() const -> T { return get_cache().cast<T>(); }

	protected:
		NODISCARD obj_type & get_cache() const { return ((!!m_cache) || (m_cache = Policy::get(m_obj, m_key))), m_cache; }

	private:
		obj_type m_obj;
		key_type m_key;
		mutable obj_type m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// attribute accessor
	template <class T> struct accessor_policies::AttrPolicy
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static auto get(O && o, Index && i) { return api::getattr(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { api::setattr(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// item accessor
	template <class T> struct accessor_policies::ItemPolicy
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static auto get(O && o, Index && i) { return api::getitem(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { api::setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
