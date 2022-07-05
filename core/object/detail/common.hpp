#ifndef _ISM_COMMON_HPP_
#define _ISM_COMMON_HPP_

#include <core/typedefs.hpp>
#include <core/os/safe_ref_count.hpp>
#include <core/string/path.hpp>
#include <core/string/print_string.hpp>
#include <core/math/transform_2d.hpp>
#include <core/templates/buffer.hpp>
#include <core/templates/duration.hpp>
#include <core/templates/ecs.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>

#ifndef MAX_ARGUMENTS
#define MAX_ARGUMENTS 24
#endif

#define FWD_OBJ(expr) \
	(ism::object_or_cast(FWD(expr)))

#define STR_IDENTIFIER(m_name) \
	static ism::StringObject CAT(ID_, m_name) { TOSTR(m_name) }

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Internals;

	template <class T> class EmbedObjectClassHelper;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// API

	struct _API_Tag { /* used to determine if type derives ObjectAPI */ };
	
	template <class T
	> constexpr bool is_api_v{ std::is_base_of_v<_API_Tag, mpl::intrinsic_t<T>> };

	template <class Derived> class ObjectAPI;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// OBJECT

	class Object;
	class TypeObject;
	class IntObject;
	class FloatObject;
	class StringObject;
	class TupleObject;
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

	// REF

	struct _Ref_Tag { /* used to determine if type derives Ref */ };

	template <class T
	> constexpr bool is_ref_v{ std::is_base_of_v<_Ref_Tag, mpl::intrinsic_t<T>> };

	template <class T> class Ref;

	class OBJ;
	class TYPE;
	class INT;
	class FLT;
	class STR;
	class TUPLE;
	class LIST;
	class DICT;
	class CAPSULE;
	class FUNCTION;
	class METHOD;
	class PROPERTY;
	class CPP_FUNCTION;
	class MODULE;
	class GENERIC;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ACCESSOR

	template <class Policy> class Accessor;

	namespace accessor_policies
	{
		template <class I> struct AttrPolicy;
		template <class I> struct ItemPolicy;
	}

	template <class I> ALIAS(AttrAccessor) Accessor<accessor_policies::AttrPolicy<I>>;
	template <class I> ALIAS(ItemAccessor) Accessor<accessor_policies::ItemPolicy<I>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(unaryfunc)		OBJ(*)(OBJ a);
	ALIAS(binaryfunc)		OBJ(*)(OBJ a, OBJ b);
	ALIAS(ternaryfunc)		OBJ(*)(OBJ a, OBJ b, OBJ c);

	ALIAS(inquiry)			bool(*)(OBJ obj);
	ALIAS(sizeargfunc)		OBJ(*)(OBJ obj, ssize_t i);
	ALIAS(sizesizeargfunc)	OBJ(*)(OBJ obj, ssize_t i, ssize_t j);
	ALIAS(objobjproc)		int32_t(*)(OBJ lhs, OBJ rhs);

	ALIAS(getattrfunc)		OBJ(*)(OBJ obj, cstring name);
	ALIAS(setattrfunc)		Error_(*)(OBJ obj, cstring name, OBJ value);
	ALIAS(getattrofunc)		OBJ(*)(OBJ obj, OBJ name);
	ALIAS(setattrofunc)		Error_(*)(OBJ obj, OBJ name, OBJ value);
	ALIAS(descrgetfunc)		OBJ(*)(OBJ descr, OBJ obj, OBJ cls);
	ALIAS(descrsetfunc)		Error_(*)(OBJ descr, OBJ obj, OBJ value);

	ALIAS(installerfunc)	TYPE(*)(TYPE type);
	ALIAS(newfunc)			OBJ(*)(TYPE type, OBJ args);
	ALIAS(delfunc)			void(*)(Object * ptr);
	ALIAS(cmpfunc)			int32_t(*)(OBJ lhs, OBJ rhs);
	ALIAS(hashfunc)			hash_t(*)(OBJ obj);
	ALIAS(lenfunc)			ssize_t(*)(OBJ obj);
	ALIAS(reprfunc)			STR(*)(OBJ obj);
	ALIAS(vectorcallfunc)	OBJ(*)(OBJ self, OBJ const * argc, size_t argv);
	ALIAS(getter)			OBJ(*)(OBJ obj, void * closure);
	ALIAS(setter)			Error_(*)(OBJ obj, OBJ value, void * closure);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(TypeFlags) int32_t;
	enum TypeFlags_ : TypeFlags
	{
		TypeFlags_None,

		TypeFlags_HaveVectorCall	= 1 << 0,
		TypeFlags_Ready				= 1 << 1,
		TypeFlags_Readying			= 1 << 2,
		TypeFlags_MethodDescriptor	= 1 << 3,
		TypeFlags_IsAbstract		= 1 << 4,
		TypeFlags_IsFinal			= 1 << 5,

		TypeFlags_Int_Subclass		= 1 << 24,
		TypeFlags_Float_Subclass	= 1 << 25,
		TypeFlags_Str_Subclass		= 1 << 26,
		TypeFlags_Tuple_Subclass	= 1 << 27,
		TypeFlags_List_Subclass		= 1 << 28,
		TypeFlags_Dict_Subclass		= 1 << 29,
		TypeFlags_Type_Subclass		= 1 << 30,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(ReturnPolicy) int32_t;
	enum ReturnPolicy_ : ReturnPolicy
	{
		ReturnPolicy_Automatic,
		ReturnPolicy_AutomaticReference,
		ReturnPolicy_TakeOwnership,
		ReturnPolicy_Copy,
		ReturnPolicy_Move,
		ReturnPolicy_Reference,
		ReturnPolicy_ReferenceInternal,

		ReturnPolicy_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(DataType) int32_t;
	enum DataType_ : DataType
	{
		DataType_Void, // void

		DataType_Bool, DataType_Byte, DataType_Char, // small data

		DataType_I8, DataType_I16, DataType_I32, DataType_I64, // signed

		DataType_U8, DataType_U16, DataType_U32, DataType_U64, // unsigned

		DataType_F32, DataType_F64, // floating point

		DataType_String, DataType_Object, // big data
		
		DataType_MAX
	};

#if ARCHITECTURE == 32
	constexpr DataType_ DataType_SizeT{ DataType_U32 }; // size_t (32-bit)
	constexpr DataType_ DataType_SSizeT{ DataType_I32 }; // ssize_t (32-bit)
#else
	constexpr DataType_ DataType_SizeT{ DataType_U64 }; // size_t (64-bit)
	constexpr DataType_ DataType_SSizeT{ DataType_I64 }; // ssize_t (64-bit)
#endif


	constexpr size_t get_data_type_size(DataType_ type) noexcept
	{
		switch (type)
		{
		case DataType_I8:
		case DataType_U8:
		case DataType_Char:
		case DataType_Byte:
		case DataType_Bool: return 1;

		case DataType_I16:
		case DataType_U16: return 2;

		case DataType_I32:
		case DataType_U32:
		case DataType_F32: return 4;

		case DataType_I64:
		case DataType_U64:
		case DataType_F64: return 8;

		case DataType_Object: return sizeof(void *);
		case DataType_String: return sizeof(String);
		}
		return 0;
	}

	template <DataType_ type
	> constexpr size_t get_data_type_size() noexcept
	{
		return get_data_type_size(type);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(CompareOperator) int32_t;
	enum CompareOperator_ : CompareOperator
	{
		CompareOperator_Never,
		CompareOperator_Less,
		CompareOperator_Equal,
		CompareOperator_LessOrEqual,
		CompareOperator_Greater,
		CompareOperator_NotEqual,
		CompareOperator_GreaterOrEqual,
		CompareOperator_Always,
		CompareOperator_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(LogicOperation) int32_t;
	enum LogicOperation_ : LogicOperation
	{
		LogicOperation_Clear,
		LogicOperation_And,
		LogicOperation_AndReverse,
		LogicOperation_Copy,
		LogicOperation_AndInverted,
		LogicOperation_NoOp,
		LogicOperation_Xor,
		LogicOperation_Or,
		LogicOperation_Nor,
		LogicOperation_Equivalent,
		LogicOperation_Invert,
		LogicOperation_OrReverse,
		LogicOperation_CopyInverted,
		LogicOperation_OrInverted,
		LogicOperation_Nand,
		LogicOperation_Set,
		LogicOperation_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct MemberDef
	{
		cstring		name{};
		DataType_	type{};
		ssize_t		offset{};
		int32_t		flags{};
		cstring		doc{};
	};

	struct MethodDef
	{
		cstring		name{};
		binaryfunc	func{};
		int32_t		flags{};
		cstring		doc{};
	};

	struct GetSetDef
	{
		cstring		name{};
		getter		get{};
		setter		set{};
		void *		closure{};
		cstring		doc{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
