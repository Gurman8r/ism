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
#include <core/templates/ecs.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/timer.hpp>
#include <core/templates/type_info.hpp>

#define FWD_OBJ(expr) \
	(ism::object_or_cast(FWD(expr)))

#define STR_IDENTIFIER(m_name) \
	static ism::StringObject CAT(ID_, m_name){ TOSTR(m_name) }

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Internals;

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

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
	template <class T> class CustomRef;

	ALIAS(OBJ)				Ref<Object>;
	ALIAS(TYPE)				CustomRef<TypeObject>;
	ALIAS(INT)				CustomRef<IntObject>;
	ALIAS(FLT)				CustomRef<FloatObject>;
	ALIAS(STR)				CustomRef<StringObject>;
	ALIAS(LIST)				CustomRef<ListObject>;
	ALIAS(DICT)				CustomRef<DictObject>;
	ALIAS(CAPSULE)			CustomRef<CapsuleObject>;
	ALIAS(FUNCTION)			CustomRef<FunctionObject>;
	ALIAS(METHOD)			CustomRef<MethodObject>;
	ALIAS(PROPERTY)			CustomRef<PropertyObject>;
	ALIAS(CPP_FUNCTION)		CustomRef<CppFunctionObject>;
	ALIAS(MODULE)			CustomRef<ModuleObject>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy> class Accessor;
	namespace accessor_policies
	{
		template <class I> struct Attr;
		template <class I> struct Item;
	}
	template <class I> ALIAS(AttrAccessor) Accessor<accessor_policies::Attr<I>>;
	template <class I> ALIAS(ItemAccessor) Accessor<accessor_policies::Item<I>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// enum types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_ENUM(DataType)
	{
		DataType_Invalid = -1,
		DataType_Int8, DataType_Int16, DataType_Int32, DataType_Int64,
		DataType_Uint8, DataType_Uint16, DataType_Uint32, DataType_Uint64,
		DataType_Float, DataType_Double,
		DataType_String, DataType_Object,
		DataType_Char, DataType_Byte, DataType_Bool,
		DataType_None,

#if ISM_ARCH < 64
		DataType_SizeT = DataType_Uint32,
		DataType_SSizeT = DataType_Int32,
#else
		DataType_SizeT = DataType_Uint64,
		DataType_SSizeT = DataType_Int64,
#endif
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_ENUM(ReturnPolicy)
	{
		ReturnPolicy_Automatic,
		ReturnPolicy_AutomaticReference,
		ReturnPolicy_TakeOwnership,
		ReturnPolicy_Copy,
		ReturnPolicy_Move,
		ReturnPolicy_Reference,
		ReturnPolicy_ReferenceInternal,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_ENUM(TypeFlags)
	{
		TypeFlags_None = 0,

		TypeFlags_HeapType			= 1 << 0,
		TypeFlags_BaseType			= 1 << 1,
		TypeFlags_HaveVectorCall	= 1 << 2,
		TypeFlags_Ready				= 1 << 3,
		TypeFlags_Readying			= 1 << 4,
		TypeFlags_MethodDescriptor	= 1 << 5,
		TypeFlags_IsAbstract		= 1 << 6,
		TypeFlags_IsFinal			= 1 << 7,
		TypeFlags_IsLocal			= 1 << 8,

		TypeFlags_Int_Subclass		= 1 << 25,
		TypeFlags_Float_Subclass	= 1 << 26,
		TypeFlags_Str_Subclass		= 1 << 27,
		TypeFlags_List_Subclass		= 1 << 28,
		TypeFlags_Dict_Subclass		= 1 << 29,
		TypeFlags_Type_Subclass		= 1 << 30,

		TypeFlags_Default = TypeFlags_None,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_ENUM(MemberFlags)
	{
		MemberFlags_None			= 0,
		MemberFlags_ReadOnly		= 1 << 0,
		MemberFlags_ReadRestricted	= 1 << 1,
		MemberFlags_WriteRestricted	= 1 << 2,
		MemberFlags_Restricted		= MemberFlags_ReadRestricted | MemberFlags_WriteRestricted,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_ENUM(MethodFlags)
	{
		MethodFlags_None		= 1 << 0,
		MethodFlags_VarArgs		= 1 << 1,
		MethodFlags_KeyArgs		= 1 << 2,
		MethodFlags_NoArgs		= 1 << 3,
		MethodFlags_O			= 1 << 4,
		MethodFlags_Class		= 1 << 5,
		MethodFlags_Static		= 1 << 6,
		MethodFlags_Coexist		= 1 << 7,
		MethodFlags_FastCall	= 1 << 8,
		MethodFlags_Method		= 1 << 9,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// function types
namespace ism
{
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
	ALIAS(descrgetfunc)		OBJ(*)(OBJ descr, OBJ obj, OBJ type);
	ALIAS(descrsetfunc)		Error(*)(OBJ descr, OBJ obj, OBJ value);
	
	ALIAS(bindfunc)			TYPE(*)(TYPE type);
	ALIAS(newfunc)			OBJ(*)(TYPE type, OBJ args);
	ALIAS(delfunc)			void(*)(Object * ptr);
	ALIAS(cmpfunc)			int32_t(*)(OBJ lhs, OBJ rhs);
	ALIAS(hashfunc)			hash_t(*)(OBJ obj);
	ALIAS(lenfunc)			ssize_t(*)(OBJ obj);
	ALIAS(reprfunc)			STR(*)(OBJ obj);
	ALIAS(vectorcallfunc)	OBJ(*)(OBJ self, OBJ const * argc, size_t argv);

	ALIAS(cfunction)		OBJ(*)(OBJ self, OBJ args);
	ALIAS(freefunc)			void(*)(void * ptr);
	ALIAS(getter)			OBJ(*)(OBJ self, void * context);
	ALIAS(setter)			Error(*)(OBJ self, OBJ value, void * context);
	ALIAS(wrapperfunc)		OBJ(*)(OBJ self, OBJ args, void * wrapped);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD NumberMethods final
	{
		binaryfunc	operator_add				{};
		binaryfunc	operator_subtract			{};
		binaryfunc	operator_multiply			{};
		binaryfunc	operator_divide				{};
		binaryfunc	operator_remainder			{};
		ternaryfunc	operator_power				{};
		binaryfunc	operator_lshift				{};
		binaryfunc	operator_rshift				{};
		binaryfunc	operator_and				{};
		binaryfunc	operator_xor				{};
		binaryfunc	operator_or					{};

		binaryfunc	operator_inplace_add		{};
		binaryfunc	operator_inplace_subtract	{};
		binaryfunc	operator_inplace_multiply	{};
		binaryfunc	operator_inplace_divide		{};
		binaryfunc	operator_inplace_remainder	{};
		ternaryfunc	operator_inplace_power		{};
		binaryfunc	operator_inplace_lshift		{};
		binaryfunc	operator_inplace_rshift		{};
		binaryfunc	operator_inplace_and		{};
		binaryfunc	operator_inplace_xor		{};
		binaryfunc	operator_inplace_or			{};

		unaryfunc	operator_positive			{};
		unaryfunc	operator_negative			{};
		unaryfunc	operator_absolute			{};
		inquiry		operator_bool				{};
		unaryfunc	operator_invert				{};
		unaryfunc	operator_integer			{};
		unaryfunc	operator_decimal			{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD SequenceMethods final
	{
		lenfunc		sequence_length		{};
		sizeargfunc	sequence_item		{};
		objobjproc	sequence_contains	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MappingMethods final
	{
		lenfunc		mapping_length		{};
		binaryfunc	mapping_subscript	{};
	};

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

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJ operator()(Args && ... args) const; // call.hpp

	public:
		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }

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
			else if (cmpfunc cmp{ typeof(self)->tp_cmp })
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

	template <class T> struct Hash<ObjectAPI<T>>
	{
		template <class U> hash_t operator()(U const & o) const { return hash(o); }
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

		Ref() noexcept {}

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
			if (m_ptr && m_ptr->unreference()) { ism::default_delete(m_ptr); }
			
			m_ptr = nullptr;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }

		NODISCARD auto ptr() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD auto operator*() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD auto operator->() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD bool operator==(value_type const * value) const noexcept { return (m_ptr == value) || ((m_ptr && value) && m_ptr->equal_to(*value)); }

		NODISCARD bool operator!=(value_type const * value) const noexcept { return (m_ptr != value) && ((m_ptr && value) && m_ptr->not_equal_to(*value)); }
		
		NODISCARD bool operator==(Ref const & value) const noexcept { return operator==(*value); }
		
		NODISCARD bool operator!=(Ref const & value) const noexcept { return operator!=(*value); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Ref<T>> : Hash<ObjectAPI<Ref<T>>> {};

	template <class T> struct EqualTo<Ref<T>> : EqualTo<ObjectAPI<Ref<T>>> {};

	template <class T> struct NotEqualTo<Ref<T>> : NotEqualTo<ObjectAPI<Ref<T>>> {};

	template <class T> struct Less<Ref<T>> : Less<ObjectAPI<Ref<T>>> {};

	template <class T> struct Greater<Ref<T>> : Greater<ObjectAPI<Ref<T>>> {};

	template <class T> struct LessEqual<Ref<T>> : LessEqual<ObjectAPI<Ref<T>>> {};

	template <class T> struct GreaterEqual<Ref<T>> : GreaterEqual<ObjectAPI<Ref<T>>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// custom ref
#define DECL_CUSTOM_REF(m_class) \
	template <> class ism::CustomRef<m_class> : public ism::Ref<m_class>

	// ref common
#define REF_COMMON(m_class, m_check)																		\
public:																										\
	using value_type = typename m_class;																	\
																											\
	NODISCARD static bool check_(OBJ const & o) { return o && (bool)(m_check(o)); }							\
																											\
	NODISCARD bool check() const { return m_ptr && (bool)(m_check(m_ptr)); }								\
																											\
	~CustomRef() noexcept = default;																		\
																											\
	CustomRef() noexcept = default;																			\
																											\
	CustomRef(nullptr_t) noexcept {}																		\
																											\
	CustomRef(m_class * value) { if (value) { ref_pointer(value); } }										\
																											\
	CustomRef(ism::Ref<m_class> const & value) { ref(value); }												\
																											\
	template <class U> CustomRef(ism::Ref<U> const & value) { reset(value); }								\
																											\
	CustomRef(m_class && value) noexcept { instance(std::move(value)); }									\
																											\
	CustomRef & operator=(nullptr_t) { return unref(), (*this); }											\
																											\
	CustomRef & operator=(ism::Ref<m_class> const & value) { return reset(value), (*this); }				\
																											\
	template <class U> CustomRef & operator=(ism::Ref<U> const & value) { return reset(value), (*this); }	\
																											\
	CustomRef & operator=(m_class && value) noexcept { return instance(std::move(value)), (*this); }		\
																											\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// no check
#define OBJECT_NO_CHECK(o) (false)

	// default handle
	template <class T> class CustomRef : public Ref<T>
	{
		REF_COMMON(T, OBJECT_NO_CHECK);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<CustomRef<T>> : Hash<Ref<T>> {};

	template <class T> struct EqualTo<CustomRef<T>> : EqualTo<Ref<T>> {};

	template <class T> struct NotEqualTo<CustomRef<T>> : NotEqualTo<Ref<T>> {};

	template <class T> struct Less<CustomRef<T>> : Less<Ref<T>> {};

	template <class T> struct Greater<CustomRef<T>> : Greater<Ref<T>> {};

	template <class T> struct LessEqual<CustomRef<T>> : LessEqual<Ref<T>> {};

	template <class T> struct GreaterEqual<CustomRef<T>> : GreaterEqual<Ref<T>> {};

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
		using key_type = typename Policy::key_type;

		Accessor(OBJ const & obj, key_type const & key) : m_obj{ obj }, m_key{ key } {}

		Accessor(Accessor const &) = default;

		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJ(a)); }

		void operator=(Accessor const & a) & { operator=(OBJ(a)); }

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
		OBJ & get_cache() const { return ((!!m_cache) || (m_cache = Policy::get(m_obj, m_key))), m_cache; }

	private:
		OBJ m_obj;
		key_type m_key;
		mutable OBJ m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// attribute accessor
	template <class T> struct accessor_policies::Attr
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static OBJ get(O && o, Index && i) { return getattr(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { setattr(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// item accessor
	template <class T> struct accessor_policies::Item
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static OBJ get(O && o, Index && i) { return getitem(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
