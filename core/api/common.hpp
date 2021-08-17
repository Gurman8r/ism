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

#define IMPLEMENT_CLASS(m_class, m_var, ...) \
	DECLEXPR(m_class::_class_type_static) = COMPOSE(ism::TypeObject, m_var, ##__VA_ARGS__)

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(InstanceID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class TypeDB;

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _API_Tag {};
	
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
	class GenericObject;

	template <class T
	> constexpr bool is_base_object_v{ std::is_base_of_v<Object, mpl::intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _Ref_Tag {};

	template <class T
	> constexpr bool is_ref_v{ std::is_base_of_v<_Ref_Tag, mpl::intrinsic_t<T>> };

	template <class T> class Ref;
	template <class T> class Handle;

	ALIAS(OBJ)				Handle<Object>;
	ALIAS(TYPE)				Handle<TypeObject>;
	ALIAS(INT)				Handle<IntObject>;
	ALIAS(FLT)				Handle<FloatObject>;
	ALIAS(STR)				Handle<StringObject>;
	ALIAS(LIST)				Handle<ListObject>;
	ALIAS(DICT)				Handle<DictObject>;
	ALIAS(CAPSULE)			Handle<CapsuleObject>;
	ALIAS(FUNCTION)			Handle<FunctionObject>;
	ALIAS(METHOD)			Handle<MethodObject>;
	ALIAS(PROPERTY)			Handle<PropertyObject>;
	ALIAS(CPP_FUNCTION)		Handle<CppFunctionObject>;
	ALIAS(MODULE)			Handle<ModuleObject>;
	ALIAS(GENERIC)			Handle<GenericObject>;

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

	class StackFrame;
	class InterpreterState;
	class RuntimeState;
	class ThreadState;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum DataType_ : int32_t
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
	}
	DataType;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum ReturnPolicy_ : uint8_t
	{
		ReturnPolicy_Automatic,
		ReturnPolicy_AutomaticReference,
		ReturnPolicy_TakeOwnership,
		ReturnPolicy_Copy,
		ReturnPolicy_Move,
		ReturnPolicy_Reference,
		ReturnPolicy_ReferenceInternal,
	}
	ReturnPolicy;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum TypeFlags_ : int32_t
	{
		TypeFlags_None = 0,

		TypeFlags_HeapType			 = 1 << 0,
		TypeFlags_BaseType			 = 1 << 1,
		TypeFlags_HaveVectorCall	 = 1 << 2,
		TypeFlags_Ready				 = 1 << 3,
		TypeFlags_Readying			 = 1 << 4,
		TypeFlags_HaveGc			 = 1 << 5,
		TypeFlags_MethodDescriptor	 = 1 << 6,
		TypeFlags_IsAbstract		 = 1 << 7,

		TypeFlags_Int_Subclass		= 1 << 25,
		TypeFlags_Float_Subclass	= 1 << 26,
		TypeFlags_Str_Subclass		= 1 << 27,
		TypeFlags_List_Subclass		= 1 << 28,
		TypeFlags_Dict_Subclass		= 1 << 29,
		TypeFlags_Type_Subclass		= 1 << 30,

		TypeFlags_Default = TypeFlags_None,
	}
	TypeFlags;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	typedef enum MethodFlags_ : int32_t
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
	}
	MethodFlags;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		OBJ(*)(OBJ a);
	ALIAS(binaryfunc)		OBJ(*)(OBJ a, OBJ b);
	ALIAS(ternaryfunc)		OBJ(*)(OBJ a, OBJ b, OBJ c);
	
	ALIAS(inquiry)			bool(*)(OBJ o);
	ALIAS(sizeargfunc)		OBJ(*)(OBJ o, ssize_t i);
	ALIAS(sizesizeargfunc)	OBJ(*)(OBJ o, ssize_t i, ssize_t j);
	ALIAS(objobjproc)		int32_t(*)(OBJ a, OBJ b);

	ALIAS(visitproc)		void(*)(OBJ, void *);
	ALIAS(traverseproc)		void(*)(OBJ, visitproc, void *);

	ALIAS(getattrfunc)		OBJ(*)(OBJ obj, cstring name);
	ALIAS(setattrfunc)		Error(*)(OBJ obj, cstring name, OBJ value);
	ALIAS(getattrofunc)		OBJ(*)(OBJ obj, OBJ name);
	ALIAS(setattrofunc)		Error(*)(OBJ obj, OBJ name, OBJ value);
	ALIAS(descrgetfunc)		OBJ(*)(OBJ descr, OBJ obj, OBJ type);
	ALIAS(descrsetfunc)		Error(*)(OBJ descr, OBJ obj, OBJ value);

	ALIAS(cmpfunc)			int32_t(*)(OBJ a, OBJ b);
	ALIAS(hashfunc)			hash_t(*)(OBJ o);
	ALIAS(lenfunc)			ssize_t(*)(OBJ o);
	ALIAS(reprfunc)			STR(*)(OBJ o);

	ALIAS(allocfunc)		void * (*)(size_t size);
	ALIAS(freefunc)			void(*)(void * ptr);
	ALIAS(initproc)			Error(*)(OBJ self, OBJ args);
	ALIAS(newfunc)			OBJ(*)(TYPE type, OBJ args);
	ALIAS(destructor)		void(*)(Object * ptr);

	ALIAS(cfunction)		OBJ(*)(OBJ self, OBJ args);
	ALIAS(vectorcallfunc)	OBJ(*)(OBJ self, OBJ const * argc, size_t argv);
	ALIAS(getter)			OBJ(*)(OBJ self, void * context);
	ALIAS(setter)			Error(*)(OBJ self, OBJ value, void * context);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD NumberMethods
	{
		binaryfunc	operator_add{};
		binaryfunc	operator_subtract{};
		binaryfunc	operator_multiply{};
		binaryfunc	operator_divide{};
		binaryfunc	operator_remainder{};
		ternaryfunc	operator_power{};
		binaryfunc	operator_lshift{};
		binaryfunc	operator_rshift{};
		binaryfunc	operator_and{};
		binaryfunc	operator_xor{};
		binaryfunc	operator_or{};

		binaryfunc	operator_inplace_add{};
		binaryfunc	operator_inplace_subtract{};
		binaryfunc	operator_inplace_multiply{};
		binaryfunc	operator_inplace_divide{};
		binaryfunc	operator_inplace_remainder{};
		ternaryfunc	operator_inplace_power{};
		binaryfunc	operator_inplace_lshift{};
		binaryfunc	operator_inplace_rshift{};
		binaryfunc	operator_inplace_and{};
		binaryfunc	operator_inplace_xor{};
		binaryfunc	operator_inplace_or{};

		unaryfunc	operator_positive{};
		unaryfunc	operator_negative{};
		unaryfunc	operator_absolute{};
		inquiry		operator_bool{};
		unaryfunc	operator_invert{};
		unaryfunc	operator_integer{};
		unaryfunc	operator_decimal{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD SequenceMethods
	{
		lenfunc		sequence_length{};
		sizeargfunc	sequence_item{};
		objobjproc	sequence_contains{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MappingMethods
	{
		lenfunc		mapping_length{};
		binaryfunc	mapping_subscript{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object_api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : _API_Tag
	{
	private:
		NODISCARD auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		NODISCARD auto handle() const & noexcept
		{
			if constexpr (is_base_object_v<Derived>)
			{
				return Handle<Derived>{ derived().ptr() };
			}
			else
			{
				return derived();
			}
		}

		template <class Index = cstring
		> NODISCARD auto attr(Index && i) const { return AttrAccessor<Index>{ handle(), FWD(i) }; }

		template <class Index = cstring
		> NODISCARD auto operator[](Index && i) const { return ItemAccessor<Index>{ handle(), FWD(i) }; }

		template <class Value = OBJ
		> NODISCARD bool contains(Value && v) const { return attr("__contains__")(FWD(v)).cast<bool>(); }

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJ operator()(Args && ... args) const; // call.hpp

		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }

		NODISCARD bool is(ObjectAPI const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }

		template <class O, class = std::enable_if_t<is_base_object_v<O>>
		> NODISCARD bool is(O const * o) const noexcept { return derived().ptr() == o; }

		template <class O, class = std::enable_if_t<is_api_v<O>>
		> NODISCARD bool is(O const & o) const noexcept { return derived().ptr() == o.ptr(); }
		
		NODISCARD bool equal_to(ObjectAPI const & o) const noexcept { return compare(o) == 0; }
		
		NODISCARD bool not_equal_to(ObjectAPI const & o) const noexcept { return compare(o) != 0; }
		
		NODISCARD bool less(ObjectAPI const & o) const noexcept { return compare(o) < 0; }
		
		NODISCARD bool less_equal(ObjectAPI const & o) const noexcept { return compare(o) <= 0; }
		
		NODISCARD bool greater(ObjectAPI const & o) const noexcept { return compare(o) > 0; }
		
		NODISCARD bool greater_equal(ObjectAPI const & o) const noexcept { return compare(o) >= 0; }

		NODISCARD auto doc() const { return attr("__doc__"); }

	private:
		NODISCARD auto compare(ObjectAPI const & o) const
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
		template <class U
		> hash_t operator()(U const & o) const { return hash(o); }
	};

	template <class T> struct EqualTo<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<ObjectAPI<T>>
	{
		template <class A, class B
		> bool operator()(A const & a, B const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ref
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Type
	> class Ref : public _Ref_Tag, public ObjectAPI<Ref<_Type>>
	{
	public:
		using value_type = typename _Type;

		using self_type = typename Ref<value_type>;

		~Ref() noexcept { unref(); }

		Ref() noexcept {}

		Ref(nullptr_t) noexcept {}

		Ref(value_type * value) { if (value) { ref_pointer(value); } }

		Ref(self_type const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(value_type const & value) { instance(value); }

		Ref(value_type && value) noexcept { instance(std::move(value)); }

		self_type & operator=(nullptr_t) { unref(); return (*this); }

		self_type & operator=(self_type const & value) { reset(value); return (*this); }

		template <class U
		> self_type & operator=(Ref<U> const & value) { reset(value); return (*this); }

		self_type & operator=(value_type const & value) { instance(value); return (*this); }

		self_type & operator=(value_type && value) noexcept { instance(std::move(value)); return (*this); }

	public:
		template <class ... Args
		> static auto new_(Args && ... args) { return Ref<value_type>{ value_type{ FWD(args)... } }; }

		template <class U> NODISCARD U cast() const &; // cast.hpp

		template <class U> NODISCARD U cast() &&; // cast.hpp

	public:
		template <class ... Args
		> void instance(Args && ... args)
		{
			ref(ism::construct_or_initialize<value_type>(FWD(args)...));
		}

		void unref()
		{
			if (m_ptr && m_ptr->dec_ref()) { ism::default_delete(m_ptr); }
			
			m_ptr = nullptr;
		}

		void reset(self_type const & value)
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

		value_type * release() noexcept
		{
			value_type * temp{ m_ptr };
			m_ptr = nullptr;
			return temp;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }

		NODISCARD auto ptr() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD auto operator*() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD auto operator->() const noexcept { return const_cast<value_type *>(m_ptr); }

		NODISCARD bool operator==(value_type const * value) const noexcept { return m_ptr == value; }
		
		NODISCARD bool operator!=(value_type const * value) const noexcept { return m_ptr != value; }
		
		NODISCARD bool operator<(self_type const & value) const noexcept { return m_ptr < value.m_ptr; }
		
		NODISCARD bool operator==(self_type const & value) const noexcept { return m_ptr == value.m_ptr; }
		
		NODISCARD bool operator!=(self_type const & value) const noexcept { return m_ptr != value.m_ptr; }

	protected:
		value_type * m_ptr{};

		void ref(self_type const & value)
		{
			if (value.m_ptr == m_ptr) { return; }
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) { m_ptr->inc_ref(); }
		}

		void ref_pointer(value_type * value)
		{
			VERIFY("INVALID POINTER" && value);

			if (value->init_ref()) { m_ptr = value; }
		}
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
}

// handle
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// handle default
#define ISM_HANDLE_DEFAULT(m_class, m_check)															\
public:																									\
	using value_type = typename m_class;																\
																										\
	using base_type = typename ism::Ref<value_type>;													\
																										\
	using self_type = typename ism::Handle<value_type>;													\
																										\
	NODISCARD static bool check_(OBJ const & o) { return o && (bool)(m_check(o)); }						\
																										\
	NODISCARD bool check() const { return m_ptr && (bool)(m_check(m_ptr)); }							\
																										\
	~Handle() noexcept = default;																		\
																										\
	Handle() noexcept = default;																		\
																										\
	Handle(nullptr_t) noexcept {}																		\
																										\
	Handle(value_type * value) { if (value) { ref_pointer(value); } }									\
																										\
	Handle(base_type const & value) { ref(value); }														\
																										\
	template <class U> Handle(Ref<U> const & value) { reset(value); }									\
																										\
	Handle(value_type const & value) { instance(value); }												\
																										\
	Handle(value_type && value) noexcept { instance(std::move(value)); }								\
																										\
	self_type & operator=(nullptr_t) { unref(); return (*this); }										\
																										\
	self_type & operator=(base_type const & value) { reset(value); return (*this); }					\
																										\
	template <class U> self_type & operator=(Ref<U> const & value) { reset(value); return (*this); }	\
																										\
	self_type & operator=(value_type const & value) { instance(value); return (*this); }				\
																										\
	self_type & operator=(value_type && value) noexcept { instance(std::move(value)); return (*this); }	\
																										\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Handle<T>> : Hash<Ref<T>> {};

	template <class T> struct EqualTo<Handle<T>> : EqualTo<Ref<T>> {};

	template <class T> struct NotEqualTo<Handle<T>> : NotEqualTo<Ref<T>> {};

	template <class T> struct Less<Handle<T>> : Less<Ref<T>> {};

	template <class T> struct Greater<Handle<T>> : Greater<Ref<T>> {};

	template <class T> struct LessEqual<Handle<T>> : LessEqual<Ref<T>> {};

	template <class T> struct GreaterEqual<Handle<T>> : GreaterEqual<Ref<T>> {};

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

		Accessor(OBJ obj, key_type key) : m_obj{ obj }, m_key{ key } {}

		Accessor(Accessor const &) = default;

		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJ(a)); }

		void operator=(Accessor const & a) & { operator=(OBJ(a)); }

		template <class Value> decltype(auto) operator=(Value && value) &&
		{
			Policy::set(m_obj, m_key, FWD_OBJ(value));
			return (*this);
		}

		template <class Value> decltype(auto) operator=(Value && value) &
		{
			get_cache() = FWD_OBJ(value);
			return (*this);
		}

		NODISCARD auto ptr() const { return const_cast<Object *>(get_cache().ptr()); }

		template <class T> NODISCARD operator Handle<T>() const { return get_cache(); }

		template <class T> NODISCARD auto cast() const -> T { return get_cache().cast<T>(); }

	protected:
		OBJ & get_cache() const {
			if (!m_cache) { m_cache = Policy::get(m_obj, m_key); }
			return m_cache;
		}

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
