#ifndef _ISM_COMMON_HPP_
#define _ISM_COMMON_HPP_

#include <core/api/reference.hpp>

#define FWD_OBJ(expr) \
	(ism::object_or_cast(FWD(expr)))

#define ISM_OBJECT_TYPE_STATIC(m_class, m_var) \
	DECLEXPR(m_class::ob_type_static) = COMPOSE(ism::TypeObject, m_var)

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class TypeDB;

	template <class T
	> ALIAS(TypeMap) HashMap<std::type_index, T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	class MethodObject;
	class PropertyObject;
	class CppFunctionObject;
	class ModuleObject;
	class GenericObject;

	template <class T> constexpr bool is_base_object_v{ std::is_base_of_v<BaseObject, mpl::intrinsic_t<T>> };

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
	ALIAS(METHOD)			Handle<MethodObject>;
	ALIAS(PROPERTY)			Handle<PropertyObject>;
	ALIAS(CPP_FUNCTION)		Handle<CppFunctionObject>;
	ALIAS(MODULE)			Handle<ModuleObject>;
	ALIAS(GENERIC)			Handle<GenericObject>;

	template <class T> constexpr bool is_handle_v{ is_ref_v<T> && is_object_api_v<T> };

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

		template <class Value = OBJECT
		> NODISCARD bool contains(Value && v) const { return attr("__contains__")(FWD(v)).cast<bool>(); }

		template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
		> OBJECT operator()(Args && ... args) const; // call.hpp

		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }

		NODISCARD bool is(ObjectAPI const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }

		template <class O, class = std::enable_if_t<is_base_object_v<O>>
		> NODISCARD bool is(O const * o) const noexcept { return derived().ptr() == o; }

		template <class O, class = std::enable_if_t<is_object_api_v<O>>
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
}

// handle
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> class NOVTABLE BaseHandle : public Ref<T>, public ObjectAPI<BaseHandle<T>>
	{
	protected:
		BaseHandle() noexcept = default;

	public:
		~BaseHandle() noexcept = default;

		template <class T> NODISCARD T cast() const &;

		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_HANDLE_DEFAULT(m_class, m_check)														\
public:																								\
	NODISCARD static bool check_(OBJECT const & o) { return o && (bool)(m_check(o)); }				\
																									\
	NODISCARD bool check() const { return m_ptr && (bool)(m_check(m_ptr)); }						\
																									\
	~Handle() noexcept = default;																	\
																									\
	Handle() noexcept = default;																	\
																									\
	Handle(nullptr_t) {}																			\
																									\
	Handle(m_class * value) { if (value) { ref_pointer(value); } }									\
																									\
	Handle(Reference * value) { if (value) { reset(value); } }										\
																									\
	Handle(Ref<m_class> const & value) { ref(value); }												\
																									\
	template <class U> Handle(Ref<U> const & value) { reset(value); }								\
																									\
	Handle(m_class const & value) { instance(value); }												\
																									\
	Handle(m_class && value) noexcept { instance(std::move(value)); }								\
																									\
	Handle & operator=(nullptr_t) { unref(); return (*this); }										\
																									\
	Handle & operator=(Ref<m_class> const & value) { reset(value); return (*this); }				\
																									\
	template <class U> Handle & operator=(Ref<U> const & value) { reset(value); return (*this); }	\
																									\
	Handle & operator=(m_class const & value) { instance(value); return (*this); }					\
																									\
	Handle & operator=(m_class && value) noexcept { instance(std::move(value)); return (*this); }	\
																									\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Handle<T>>
	{
		hash_t operator()(Handle<T> const & o) const { return hash(o); }
	};

	template <class T> struct EqualTo<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<Handle<T>>
	{
		template <class U> bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

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

		Accessor(OBJECT obj, key_type key) : m_obj{ obj }, m_key{ key } {}

		Accessor(Accessor const &) = default;

		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT(a)); }

		void operator=(Accessor const & a) & { operator=(OBJECT(a)); }

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

		NODISCARD auto ptr() const { return const_cast<BaseObject *>(get_cache().ptr()); }

		template <class T> NODISCARD operator Handle<T>() const { return get_cache(); }

		template <class T> NODISCARD auto cast() const -> T { return get_cache().cast<T>(); }

	protected:
		OBJECT & get_cache() const {
			if (!m_cache) { m_cache = Policy::get(m_obj, m_key); }
			return m_cache;
		}

	private:
		OBJECT m_obj;
		key_type m_key;
		mutable OBJECT m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// attribute accessor
	template <class T> struct accessor_policies::Attr
	{
		using key_type = T;

		template <class O = OBJECT, class Index = T
		> static OBJECT get(O && o, Index && i) { return getattr(FWD(o), FWD(i)); }

		template <class O = OBJECT, class Index = T, class Value = OBJECT
		> static void set(O && o, Index && i, Value && v) { setattr(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// item accessor
	template <class T> struct accessor_policies::Item
	{
		using key_type = T;

		template <class O = OBJECT, class Index = T
		> static OBJECT get(O && o, Index && i) { return getitem(FWD(o), FWD(i)); }

		template <class O = OBJECT, class Index = T, class Value = OBJECT
		> static void set(O && o, Index && i, Value && v) { setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMMON_HPP_
