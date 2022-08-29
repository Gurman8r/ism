#ifndef _ISM_COMMON_HPP_
#define _ISM_COMMON_HPP_

#include <core/extension/native_interface.hpp>
#include <core/os/safe_ref_count.hpp>
#include <core/os/time.hpp>
#include <core/math/color.hpp>
#include <core/math/rect.hpp>
#include <core/string/print_string.hpp>
#include <core/templates/batch.hpp>
#include <core/templates/buffer.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/mask.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>
#include <core/templates/views.hpp>

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

	template <class T> class _EmbedClassHelper;

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
	class IteratorObject;
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
	class ITERATOR;
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

	template <class I> using AttrAccessor = Accessor<accessor_policies::AttrPolicy<I>>;
	template <class I> using ItemAccessor = Accessor<accessor_policies::ItemPolicy<I>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using unaryfunc =		OBJ(*)(OBJ a);
	using binaryfunc =		OBJ(*)(OBJ a, OBJ b);
	using ternaryfunc =		OBJ(*)(OBJ a, OBJ b, OBJ c);

	using inquiry =			bool(*)(OBJ obj);
	using sizeargfunc =		OBJ(*)(OBJ obj, ssize_t i);
	using sizesizeargfunc =	OBJ(*)(OBJ obj, ssize_t i, ssize_t j);
	using objobjproc =		i32(*)(OBJ lhs, OBJ rhs);

	using getattrfunc =		OBJ(*)(OBJ obj, cstring name);
	using setattrfunc =		Error_(*)(OBJ obj, cstring name, OBJ value);
	using getattrofunc =		OBJ(*)(OBJ obj, OBJ name);
	using setattrofunc =		Error_(*)(OBJ obj, OBJ name, OBJ value);
	using descrgetfunc =		OBJ(*)(OBJ descr, OBJ obj, OBJ cls);
	using descrsetfunc =		Error_(*)(OBJ descr, OBJ obj, OBJ value);

	using classproc =		TYPE(*)(TYPE type);
	using newfunc =			OBJ(*)(TYPE type, OBJ args);
	using delfunc =			void(*)(Object * ptr);
	using cmpfunc =			i32(*)(OBJ lhs, OBJ rhs);
	using hashfunc =			size_t(*)(OBJ obj);
	using lenfunc =			ssize_t(*)(OBJ obj);
	using reprfunc =			STR(*)(OBJ obj);
	using vectorcallfunc =	OBJ(*)(OBJ self, OBJ const * argv, size_t argc);
	using getter =			OBJ(*)(OBJ obj, void * closure);
	using setter =			Error_(*)(OBJ obj, OBJ value, void * closure);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum TypeFlags_
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

	enum ReturnPolicy_
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

	enum DataType_
	{
		DataType_Void, // void

		DataType_Bool, DataType_Byte, DataType_Char, // small data

		DataType_I8, DataType_I16, DataType_I32, DataType_I64, // signed

		DataType_U8, DataType_U16, DataType_U32, DataType_U64, // unsigned

		DataType_F32, DataType_F64, // floating point

		DataType_String, DataType_Object, // big data
		
		DataType_MAX
	};

#if ARCHITECTURE == 64
	constexpr DataType_ DataType_SizeT{ DataType_U64 }; // size_t (64-bit)
	constexpr DataType_ DataType_SSizeT{ DataType_I64 }; // ssize_t (64-bit)
#else
	constexpr DataType_ DataType_SizeT{ DataType_U32 }; // size_t (32-bit)
	constexpr DataType_ DataType_SSizeT{ DataType_I32 }; // ssize_t (32-bit)
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

	template <DataType_ T
	> constexpr size_t get_data_type_size() noexcept {
		return get_data_type_size(T);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum CompareOperator_
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

	enum LogicOperation_
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
		i32			flags{};
		cstring		doc{};

		NODISCARD operator bool() const noexcept { return name && *name; }
	};

	struct MethodDef
	{
		cstring		name{};
		binaryfunc	func{};
		i32			flags{};
		cstring		doc{};

		NODISCARD operator bool() const noexcept { return name && *name; }
	};

	struct GetSetDef
	{
		cstring		name{};
		getter		get{};
		setter		set{};
		void *		closure{};
		cstring		doc{};

		NODISCARD operator bool() const noexcept { return name && *name; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
