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
	class CoreProperty;

	class CoreCppFunction;
	class CoreModule;
	class CoreGeneric;

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
	ALIAS(PROPERTY)			Handle<CoreProperty>;
	ALIAS(STATIC_PROPERTY)	PROPERTY;

	ALIAS(CPP_FUNCTION)		Handle<CoreCppFunction>;
	ALIAS(MODULE)			Handle<CoreModule>;
	ALIAS(GENERIC)			Handle<CoreGeneric>;

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

// method suites
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		OBJECT(*)(OBJECT a);
	ALIAS(binaryfunc)		OBJECT(*)(OBJECT a, OBJECT b);
	ALIAS(ternaryfunc)		OBJECT(*)(OBJECT a, OBJECT b, OBJECT c);
	
	ALIAS(inquiry)			bool(*)(OBJECT o);
	ALIAS(sizeargfunc)		OBJECT(*)(OBJECT o, ssize_t i);
	ALIAS(sizesizeargfunc)	OBJECT(*)(OBJECT o, ssize_t i, ssize_t j);
	ALIAS(objobjproc)		int32_t(*)(OBJECT a, OBJECT b);

	ALIAS(visitproc)		void(*)(OBJECT, void *);
	ALIAS(traverseproc)		void(*)(OBJECT, visitproc, void *);

	ALIAS(getattrfunc)		OBJECT(*)(OBJECT self, cstring name);
	ALIAS(setattrfunc)		Error(*)(OBJECT self, cstring name, OBJECT value);
	ALIAS(getattrofunc)		OBJECT(*)(OBJECT self, OBJECT name);
	ALIAS(setattrofunc)		Error(*)(OBJECT self, OBJECT name, OBJECT value);
	ALIAS(descrgetfunc)		OBJECT(*)(OBJECT, OBJECT, OBJECT);
	ALIAS(descrsetfunc)		Error(*)(OBJECT, OBJECT, OBJECT);

	ALIAS(cmpfunc)			int32_t(*)(OBJECT a, OBJECT b);
	ALIAS(hashfunc)			hash_t(*)(OBJECT o);
	ALIAS(lenfunc)			ssize_t(*)(OBJECT o);
	ALIAS(reprfunc)			STR(*)(OBJECT o);

	ALIAS(allocfunc)		OBJECT(*)(TYPE type, size_t nitems);
	ALIAS(freefunc)			void(*)(void * ptr);
	ALIAS(initproc)			Error(*)(OBJECT self, OBJECT args);
	ALIAS(newfunc)			OBJECT(*)(TYPE type, OBJECT args);
	ALIAS(destructor)		void(*)(CoreObject * ptr);

	ALIAS(cfunction)		OBJECT(*)(OBJECT self, OBJECT args);
	ALIAS(vectorcallfunc)	OBJECT(*)(OBJECT self, OBJECT const * argc, size_t argv);
	ALIAS(getter)			OBJECT(*)(OBJECT self, void * context);
	ALIAS(setter)			Error(*)(OBJECT self, OBJECT value, void * context);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD NumberMethods
	{
		CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(NumberMethods);

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
		CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(SequenceMethods);

		lenfunc		sequence_length{};
		sizeargfunc	sequence_item{};
		objobjproc	sequence_contains{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MappingMethods
	{
		CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(MappingMethods);

		lenfunc		mapping_length{};
		binaryfunc	mapping_subscript{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD MethodDef
	{
		CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(MethodDef);

		cstring		name{};
		cfunction	method{};
		cstring		doc{};

		NODISCARD constexpr operator bool() const noexcept { return name && *name; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD GetSetDef
	{
		CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(GetSetDef);

		cstring		name{};
		getter		get{};
		setter		set{};
		void *		closure{};
		cstring		doc{};

		NODISCARD constexpr operator bool() const noexcept { return name && *name; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
