#ifndef _ISM_COMMON_DETAIL_HPP_
#define _ISM_COMMON_DETAIL_HPP_

#include <core/api/reference.hpp>

// types
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ClassDB;

	struct _API_Tag {};
	
	template <class Derived> class ObjectAPI;
	
	template <class T> constexpr bool is_object_api_v{ std::is_base_of_v<_API_Tag, mpl::intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class BaseObject;
	class TypeObject;
	class IntObject;
	class FloatObject;
	class StringObject;
	class ListObject;
	class DictObject;
	class CapsuleObject;
	class FunctionObject;
	class PropertyObject;
	class CppFunctionObject;
	class ModuleObject;
	class GenericObject;

	template <class T> constexpr bool is_core_object_v{ std::is_base_of_v<BaseObject, mpl::intrinsic_t<T>> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> class BaseHandle;
	template <class T> class Handle;

	ALIAS(OBJECT)			Handle<BaseObject>;
	ALIAS(TYPE)				Handle<TypeObject>;
	ALIAS(INT)				Handle<IntObject>;
	ALIAS(FLT)				Handle<FloatObject>;
	ALIAS(STR)				Handle<StringObject>;
	ALIAS(LIST)				Handle<ListObject>;
	ALIAS(DICT)				Handle<DictObject>;
	ALIAS(CAPSULE)			Handle<CapsuleObject>;
	ALIAS(FUNCTION)			Handle<FunctionObject>;
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

	struct TypeInfo;
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
}

// method suites
namespace ism::api
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

	ALIAS(getattrfunc)		OBJECT(*)(OBJECT obj, cstring name);
	ALIAS(setattrfunc)		Error(*)(OBJECT obj, cstring name, OBJECT value);
	ALIAS(getattrofunc)		OBJECT(*)(OBJECT obj, OBJECT name);
	ALIAS(setattrofunc)		Error(*)(OBJECT obj, OBJECT name, OBJECT value);
	ALIAS(descrgetfunc)		OBJECT(*)(OBJECT descr, OBJECT obj, OBJECT type);
	ALIAS(descrsetfunc)		Error(*)(OBJECT descr, OBJECT obj, OBJECT value);

	ALIAS(cmpfunc)			int32_t(*)(OBJECT a, OBJECT b);
	ALIAS(hashfunc)			hash_t(*)(OBJECT o);
	ALIAS(lenfunc)			ssize_t(*)(OBJECT o);
	ALIAS(reprfunc)			STR(*)(OBJECT o);

	ALIAS(allocfunc)		void * (*)(size_t size);
	ALIAS(freefunc)			void(*)(void * ptr);
	ALIAS(initproc)			Error(*)(OBJECT self, OBJECT args);
	ALIAS(newfunc)			OBJECT(*)(TYPE type, OBJECT args);
	ALIAS(destructor)		void(*)(BaseObject * ptr);

	ALIAS(cfunction)		OBJECT(*)(OBJECT self, OBJECT args);
	ALIAS(vectorcallfunc)	OBJECT(*)(OBJECT self, OBJECT const * argc, size_t argv);
	ALIAS(getter)			OBJECT(*)(OBJECT self, void * context);
	ALIAS(setter)			Error(*)(OBJECT self, OBJECT value, void * context);

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

#endif // !_ISM_COMMON_DETAIL_HPP_
