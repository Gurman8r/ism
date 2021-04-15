#ifndef _ISM_COMMON_HPP_
#define _ISM_COMMON_HPP_

#include <core/api/reference.hpp>
#include <core/templates/functional.hpp>
#include <core/templates/mpl.hpp>

#define TRY_NEXT_OVERLOAD ((ism::CoreObject *)1)

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _API_Tag {};
	template <class Derived> class ObjectAPI;
	template <class T> constexpr bool is_object_api_v{ std::is_base_of_v<_API_Tag, intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Handle;
	template <class> constexpr bool is_handle_v{ false };
	template <class T> constexpr bool is_handle_v<Handle<T>>{ true };
	template <template <class> class H, class T> constexpr bool is_handle_v<H<T>>{ std::is_base_of_v<Handle<T>, H<T> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class CoreObject;
	class CoreType;
	class CoreInt;
	class CoreFloat;
	class CoreString;
	class CoreList;
	class CoreDict;
	class CoreCapsule;
	class CoreFunction;

	//class CoreMethod;
	//class CoreInstanceMethod;
	//class CoreStaticMethod;
	//class CoreProperty;

	class CoreCppFunction;
	class CoreGeneric;
	class CoreModule;

	template <class T> constexpr bool is_core_object_v{ std::is_base_of_v<CoreObject, intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(OBJECT)			Handle<CoreObject>;
	ALIAS(TYPE)				Handle<CoreType>;
	ALIAS(INT)				Handle<CoreInt>;
	ALIAS(FLT)				Handle<CoreFloat>;
	ALIAS(STR)				Handle<CoreString>;
	ALIAS(LIST)				Handle<CoreList>;
	ALIAS(DICT)				Handle<CoreDict>;
	ALIAS(CAPSULE)			Handle<CoreCapsule>;
	ALIAS(FUNCTION)			Handle<CoreFunction>;

	//ALIAS(METHOD)			Handle<CoreMethod>;
	//ALIAS(INSTANCE_METHOD)	Handle<CoreInstanceMethod>;
	//ALIAS(STATIC_METHOD)	Handle<CoreStaticMethod>;
	//ALIAS(PROPERTY)			Handle<CoreProperty>;

	ALIAS(CPP_FUNCTION)		Handle<CoreCppFunction>;
	ALIAS(GENERIC)			Handle<CoreGeneric>;
	ALIAS(MODULE)			Handle<CoreModule>;

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

	struct TypeInfo;
	class StackFrame;
	class InterpreterState;
	class RuntimeState;
	class ThreadState;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail { String get_fully_qualified_tp_name(TYPE const & t); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// enums
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class ReturnPolicy : uint8_t
	{
		Automatic,
		AutomaticReference,
		TakeOwnership,
		Copy,
		Move,
		Reference,
		ReferenceInternal,
	};

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
		MethodFlags_None,
		MethodFlags_VarArgs		= 1 << 0,
		MethodFlags_Keywords	= 1 << 1,
		MethodFlags_NoArgs		= 1 << 2,
		MethodFlags_O			= 1 << 3,
		MethodFlags_Class		= 1 << 4,
		MethodFlags_Static		= 1 << 5,
		MethodFlags_Coexist		= 1 << 6,
		MethodFlags_FastCall	= 1 << 7,
		MethodFlags_Stackless	= 1 << 8,
		MethodFlags_Method		= 1 << 9,
	}
	MethodFlags;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// method types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		StdFn<OBJECT(OBJECT a)>;
	ALIAS(binaryfunc)		StdFn<OBJECT(OBJECT a, OBJECT b)>;
	ALIAS(ternaryfunc)		StdFn<OBJECT(OBJECT a, OBJECT b, OBJECT c)>;
	ALIAS(inquiry)			StdFn<bool(OBJECT o)>;
	ALIAS(lenfunc)			StdFn<ssize_t(OBJECT o)>;

	ALIAS(getattrfunc)		StdFn<OBJECT(OBJECT self, cstring name)>;
	ALIAS(setattrfunc)		StdFn<Error(OBJECT self, cstring name, OBJECT value)>;

	ALIAS(getattrofunc)		StdFn<OBJECT(OBJECT o, OBJECT name)>;
	ALIAS(setattrofunc)		StdFn<Error(OBJECT o, OBJECT name, OBJECT value)>;

	ALIAS(descrgetfunc)		StdFn<OBJECT(OBJECT, OBJECT, OBJECT)>;
	ALIAS(descrsetfunc)		StdFn<Error(OBJECT, OBJECT, OBJECT)>;

	ALIAS(cmpfunc)			StdFn<int32_t(OBJECT a, OBJECT b)>;
	ALIAS(hashfunc)			StdFn<hash_t(OBJECT o)>;
	ALIAS(reprfunc)			StdFn<STR(OBJECT o)>;

	ALIAS(allocfunc)		StdFn<void * (size_t size)>;
	ALIAS(freefunc)			StdFn<void(void * ptr)>;
	ALIAS(initproc)			StdFn<Error(OBJECT self, OBJECT args)>;
	ALIAS(createfunc)		StdFn<OBJECT()>;

	ALIAS(cfunction)		StdFn<OBJECT(OBJECT self, OBJECT args)>;
	ALIAS(vectorcallfunc)	StdFn<OBJECT(OBJECT self, OBJECT const * args, size_t nargs)>;

	ALIAS(getter)			StdFn<OBJECT(OBJECT self, void * context)>;
	ALIAS(setter)			StdFn<Error(OBJECT self, OBJECT value, void * context)>;

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MappingMethods
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// definitions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MethodDef
	{
		String		name{};
		cfunction	method{};
		int32_t		flags{ MethodFlags_None };
		String		doc{};
	};

	template <> struct ism::Hash<MethodDef>
	{
		hash_t operator()(MethodDef const & a) const { return hashof(a.name); }
	};

	template <> struct ism::EqualTo<MethodDef>
	{
		bool operator()(MethodDef const & a, MethodDef const & b) const { return &a == &b || a.name == b.name; }

		bool operator()(MethodDef const & a, String const & i) const { return a.name == i; }
	};

	template <> struct ism::Less<MethodDef>
	{
		bool operator()(MethodDef const & a, MethodDef const & b) const { return &a != &b && a.name < b.name; }

		bool operator()(MethodDef const & a, String const & i) const { return a.name < i; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD GetSetDef
	{
		String name{};
		getter get{};
		setter set{};
		void * closure{};
		String doc{};
	};

	template <> struct ism::Hash<GetSetDef>
	{
		hash_t operator()(GetSetDef const & a) const { return hashof(a.name); }
	};

	template <> struct ism::EqualTo<GetSetDef>
	{
		bool operator()(GetSetDef const & a, GetSetDef const & b) const { return &a == &b || a.name == b.name; }

		bool operator()(GetSetDef const & a, String const & i) const { return a.name == i; }
	};

	template <> struct ism::Less<GetSetDef>
	{
		bool operator()(GetSetDef const & a, GetSetDef const & b) const { return &a != &b && a.name < b.name; }

		bool operator()(GetSetDef const & a, String const & i) const { return a.name < i; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// comparators
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class U
	> NODISCARD bool operator==(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.equal_to(b); }

	template <class T, class U
	> NODISCARD bool operator!=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.not_equal_to(b); }

	template <class T, class U
	> NODISCARD bool operator<(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.less(b); }

	template <class T, class U
	> NODISCARD bool operator<=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.less_equal(b); }

	template <class T, class U
	> NODISCARD bool operator>(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.greater(b); }

	template <class T, class U
	> NODISCARD bool operator>=(ObjectAPI<T> const & a, ObjectAPI<U> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ism::Hash<Handle<T>>
	{
		NODISCARD hash_t operator()(Handle<T> const & o) const { return o.hash(); }
	};

	template <class T> struct ism::EqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct ism::NotEqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct ism::Less<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct ism::Greater<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct ism::LessEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct ism::GreaterEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
