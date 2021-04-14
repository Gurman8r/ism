#ifndef _ISM_TYPES_HPP_
#define _ISM_TYPES_HPP_

#include <core/api/common.hpp>

// object api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : _API_Tag
	{
	private:
		NODISCARD auto derived() const noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		NODISCARD auto handle() const noexcept
		{
			if constexpr (is_core_object_v<Derived>)
			{
				return Handle<Derived>{ derived().ptr() };
			}
			else
			{
				return derived();
			}
		}

		NODISCARD auto type() const noexcept
		{
			return CHECK(derived().ptr())->ob_type;
		}

		NODISCARD auto hash() const noexcept
		{
			TYPE t{ type() };
			return t->tp_hash ? t->tp_hash(handle()) : hash_t{};
		}

		NODISCARD auto len() const noexcept
		{
			TYPE t{ type() };
			return t->tp_len ? t->tp_len(handle()) : size_t{};
		}

		NODISCARD auto repr() const noexcept
		{
			TYPE t{ type() };
			return t->tp_repr ? t->tp_repr(handle()) : STR{};
		}
		
		NODISCARD auto str() const noexcept
		{
			TYPE t{ type() };
			return t->tp_str ? t->tp_str(handle()) : STR{};
		}

		template <class I = cstring
		> NODISCARD auto attr(I && i) const { return AttrAccessor<I>{ handle(), FWD(i) }; }

		template <class I = cstring
		> NODISCARD auto operator[](I && i) const { return ItemAccessor<I>{ handle(), FWD(i) }; }

		template <class U = Derived
		> NODISCARD auto compare(ObjectAPI<U> const & o) const -> int32_t
		{
			if (TYPE t{ type() }; t->tp_compare)
			{
				return t->tp_compare(handle(), o.handle());
			}
			else if constexpr (std::is_same_v<Derived, U>)
			{
				return util::compare(derived().ptr(), o.derived().ptr());
			}
			else
			{
				return util::compare((void *)derived().ptr(), (void *)o.handle().ptr());
			}
		}

		template <class O = OBJECT
		> NODISCARD bool contains(O && v) const { return this->attr("__contains__")(FWD(v)); }

		NODISCARD bool check(TYPE const & value) const noexcept { return type().is(value); }
		NODISCARD bool is_null() const noexcept { return derived().ptr() == nullptr; }
		NODISCARD bool is_valid() const noexcept { return derived().ptr() != nullptr; }

		template <class U> NODISCARD bool is(ObjectAPI<U> const & o) const noexcept { return derived().ptr() == o.derived().ptr(); }
		template <class U> NODISCARD bool equal_to(ObjectAPI<U> const & o) const noexcept { return compare(o) == 0; }
		template <class U> NODISCARD bool not_equal_to(ObjectAPI<U> const & o) const noexcept { return compare(o) != 0; }
		template <class U> NODISCARD bool less(ObjectAPI<U> const & o) const noexcept { return compare(o) < 0; }
		template <class U> NODISCARD bool less_equal(ObjectAPI<U> const & o) const noexcept { return compare(o) <= 0; }
		template <class U> NODISCARD bool greater(ObjectAPI<U> const & o) const noexcept { return compare(o) > 0; }
		template <class U> NODISCARD bool greater_equal(ObjectAPI<U> const & o) const noexcept { return compare(o) >= 0; }

		NODISCARD auto doc() const { return this->attr("__doc__"); }

		template <class ... Args
		> OBJECT operator()(Args && ... args);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class U
	> std::ostream & operator<<(std::ostream & out, ObjectAPI<U> const & value)
	{
		TYPE t{ CHECK(value.type()) };
		auto s{ (t->tp_str ? t->tp_str(value.handle()) : (t->tp_repr ? t->tp_repr(value.handle()) : STR{})) };
		if (s) { out << s.cast<String>(); }
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// handle
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Handle : public ObjectAPI<Handle<T>>, public Ref<T>
	{
	public:
		using base_type = Ref<T>;
		using base_type::Ref;
		using base_type::operator=;
		using typename base_type::element_type;
		using typename base_type::pointer;

		NODISCARD static auto type_static() { return ism::typeof<T>(); }

		template <class ... Args
		> NODISCARD static auto create(Args && ... args) { return Handle{ memnew(T{ FWD(args)... }) }; }

		template <class T> NODISCARD T cast() const &;
		template <class T> NODISCARD T cast() &&;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class T, class SFINAE = void
		> struct handle_type { using type = OBJECT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<std::is_integral_v<T>>> { using type = INT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<std::is_floating_point_v<T>>> { using type = FLT; };

		template <class T
		> struct handle_type<T, std::enable_if_t<is_string_v<T>>> { using type = STR; };
	}

	template <class T> ALIAS(make_handle) typename impl::handle_type<intrinsic_t<T>>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// accessors
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Policy
	> struct Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using key_type = typename Policy::key_type;

		Accessor(OBJECT obj, key_type && key) : m_obj{ obj }, m_key{ FWD(key) } {}
		Accessor(Accessor const &) = default;
		Accessor(Accessor &&) = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(OBJECT{ a }); }
		void operator=(Accessor const & a) & { operator=(OBJECT{ a }); }

		template <class T> void operator=(T && v) && { Policy::set(m_obj, m_key, ism::object_or_cast(FWD(v))); }
		template <class T> void operator=(T && v) & { get_cache() = ism::object_or_cast(FWD(v)); }

		NODISCARD auto ptr() const -> CoreObject * { return get_cache().ptr(); }

		template <class T
		> NODISCARD auto cast() const -> T { return get_cache().template cast<T>(); }

		template <class O = CoreObject
		> NODISCARD operator Handle<O>() const { return get_cache(); }

	protected:
		OBJECT & get_cache() const { if (!m_cache) { m_cache = Policy::get(m_obj, m_key); } return m_cache; }

	private:
		OBJECT m_obj;
		key_type m_key;
		mutable OBJECT m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace accessor_policies
	{
		template <class T> struct Attr
		{
			using key_type = T;

			template <class O = OBJECT, class I = T
			> static OBJECT get(O && o, I && i) { return ism::getattr(FWD(o), FWD(i)); }

			template <class O = OBJECT, class I = T, class V = OBJECT
			> static void set(O && o, I && i, V && v) { ism::setattr(FWD(o), FWD(i), FWD(v)); }
		};
		
		template <class T> struct Item
		{
			using key_type = T;

			template <class O = OBJECT, class I = T
			> static OBJECT get(O && o, I && i) { return ism::getitem(FWD(o), FWD(i)); }

			template <class O = OBJECT, class I = T, class V = OBJECT
			> static void set(O && o, I && i, V && v) { ism::setitem(FWD(o), FWD(i), FWD(v)); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// object
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreObject_Type;
#define CoreObject_Type		(ism::TYPE{ &_CoreObject_Type })

	class NODISCARD ISM_API CoreObject : public ObjectAPI<CoreObject>, public Reference
	{
	protected:
		virtual void _postinitialize() override;
		virtual bool _predelete() override;

	public:
		using base_type = Reference;
		using self_type = CoreObject;

		NODISCARD static auto type_static() { return CoreObject_Type; }

		NODISCARD auto ptr() const noexcept { return const_cast<CoreObject *>(this); }

		TYPE ob_type;

	public:
		virtual ~CoreObject() override = default;
		explicit CoreObject(TYPE const & t);
		CoreObject() : self_type{ type_static() } {}
		CoreObject(self_type const &) = default;
		CoreObject(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = CoreObject, class ... Args
	> auto make_object(Args && ... args) noexcept
	{
		if constexpr (is_core_object_v<O>)
		{
			return Handle<O>::create(FWD(args)...);
		}
		else if constexpr (is_handle_v<O>)
		{
			return O::create(FWD(args)...);
		}
		else
		{
			static_assert(!"invalid instantiation");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept { return T::type_static(); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof() noexcept { return ism::typeof<make_handle<T>>(); }

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T const & o) { return o.type(); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD TYPE typeof(T && o) noexcept { return ism::typeof(ism::object_or_cast(FWD(o))); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return o.check(ism::typeof<T>());
	}

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD bool isinstance(OBJECT const & o) noexcept
	{
		return ism::isinstance<make_handle<T>>(o);
	}

	template <class A, class B = A
	> NODISCARD bool isinstance(A const & o, B const & t)
	{
		return ism::isinstance(o, ism::isinstance<TYPE>(o) ? t : t.type());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = OBJECT
	> NODISCARD OBJECT getattr(O const & o, OBJECT const & i)
	{
		TYPE t{ o.type() };
		if (!ism::isinstance<STR>(i)) { return nullptr; }
		else if (t->tp_getattro)
		{
			return t->tp_getattro(o, i);
		}
		else if (t->tp_getattr)
		{
			return t->tp_getattr(o, STR(i)->c_str());
		}
		return nullptr;
	}

	template <class O = OBJECT
	> NODISCARD OBJECT getattr(O const & o, cstring i)
	{
		TYPE t{ o.type() };
		if (t->tp_getattro)
		{
			return t->tp_getattro(o, ism::object_or_cast(i));
		}
		else if (t->tp_getattr)
		{
			return t->tp_getattr(o, i);
		}
		return nullptr;
	}

	template <class O = OBJECT, class I = OBJECT, class D = OBJECT
	> NODISCARD OBJECT getattr(O && o, I && i, D && defval) noexcept
	{
		OBJECT result{ ism::getattr(FWD(o), FWD(i)) };
		return result ? result : ism::object_or_cast(FWD(defval));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = OBJECT, class I = OBJECT
	> NODISCARD bool hasattr(O && o, I && i)
	{
		return ism::getattr(FWD(o), FWD(i)).handle().is_valid();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class V = OBJECT
	> Err setattr(OBJECT o, OBJECT const & i, V && v)
	{
		TYPE t{ o.type() };
		if (!ism::isinstance<STR>(i)) { return Err_Unknown; }
		else if (t->tp_getattro)
		{
			return t->tp_setattro(o, i, ism::object_or_cast(FWD(v)));
		}
		else if (t->tp_setattr)
		{
			return t->tp_setattr(o, STR(i)->c_str(), ism::object_or_cast(FWD(v)));
		}
		return Err_Unknown;
	}

	template <class V = OBJECT
	> Err setattr(OBJECT o, cstring i, V && v)
	{
		TYPE t{ o.type() };
		if (t->tp_getattro)
		{
			return t->tp_setattro(o, ism::object_or_cast(i), ism::object_or_cast(FWD(v)));
		}
		else if (t->tp_setattr)
		{
			return t->tp_setattr(o, i, ism::object_or_cast(FWD(v)));
		}
		return Err_Unknown;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = OBJECT, class I = OBJECT
	> NODISCARD OBJECT getitem(O && o, I && i)
	{
		OBJECT index{ object_or_cast(FWD(i)) };
		if (isinstance<DICT>(o))
		{
			VERIFY(isinstance<STR>(index));
			return (***DICT(o))[STR(index)];
		}
		else if (isinstance<LIST>(o))
		{
			VERIFY(isinstance<INT>(index));
			return (***LIST(o))[***INT(index)];
		}
		return nullptr;
	}

	template <class O = OBJECT, class I = OBJECT, class V = OBJECT
	> Err setitem(O && o, I && i, V && v)
	{
		OBJECT index{ object_or_cast(FWD(i)) };
		OBJECT value{ object_or_cast(FWD(v)) };
		if (isinstance<DICT>(o))
		{
			VERIFY(isinstance<STR>(index));
			return ((***DICT(o))[STR(index)] = value), Err_None;
		}
		else if (isinstance<LIST>(o))
		{
			VERIFY(isinstance<INT>(index));
			return ((***LIST(o))[***INT(index)] = value), Err_None;
		}
		return Err_Unknown;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_api_v<T>, int> = 0
	> NODISCARD auto object_or_cast(T && o) -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_api_v<T>, int> = 0
	> NODISCARD OBJECT object_or_cast(T && o);

	NODISCARD inline OBJECT object_or_cast(CoreObject * o) { return OBJECT{ o }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = OBJECT
	> OBJECT get_function(O const & value)
	{
		if (value)
		{
			//if (ism::isinstance<INSTANCE_METHOD>(value))
			//{
			//
			//}
			//else if (ism::isinstance<METHOD>(value))
			//{
			//}
		}
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// type
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreType_Type;
#define CoreType_Type		(ism::TYPE{ &_CoreType_Type })

	class NODISCARD ISM_API CoreType : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreType;

		NODISCARD static auto type_static() { return CoreType_Type; }

	public:
		virtual ~CoreType() override = default;
		explicit CoreType(TYPE const & t) : base_type{ t } {}
		CoreType() : base_type{ type_static() } {}
		CoreType(self_type const &) = default;
		CoreType(self_type &&) noexcept = default;
		CoreType & operator=(self_type const &) = default;
		CoreType & operator=(self_type &&) noexcept = default;

	public:
		std::type_info const *	tp_rtti;
		String					tp_name;
		ssize_t					tp_basicsize;
		int32_t					tp_flags;
		String					tp_doc;

		ssize_t					tp_dictoffset;
		ssize_t					tp_vectorcalloffset;
		ssize_t					tp_weaklistoffset;

		Optl<AsNumber>			tp_as_number;
		Optl<AsSequence>		tp_as_sequence;
		Optl<AsMapping>			tp_as_mapping;

		Vector<GetSetDef>		tp_getsets;
		Vector<MethodDef>		tp_methods;

		OBJECT					tp_call;
		cmpfunc					tp_compare;
		hashfunc				tp_hash;
		lenfunc					tp_len;
		reprfunc				tp_repr;
		reprfunc				tp_str;

		getattrfunc				tp_getattr;
		setattrfunc				tp_setattr;
		getattrofunc			tp_getattro;
		setattrofunc			tp_setattro;
		descrgetfunc			tp_descrget;
		descrsetfunc			tp_descrset;

		TYPE					tp_base;
		OBJECT					tp_bases;
		OBJECT					tp_cache;
		createfunc				tp_create;
		OBJECT					tp_dict;
		OBJECT					tp_mro;
		OBJECT					tp_subclasses;
		OBJECT					tp_weaklist;
		vectorcallfunc			tp_vectorcall;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class O = OBJECT> NODISCARD OBJECT * _get_dict_ptr(O const & o)
	{
		TYPE t{ o.type() };
		auto dictoffset{ t->tp_dictoffset };
		if (dictoffset == 0) { return nullptr; }
		VERIFY(0 < dictoffset);
		return (OBJECT *)((char *)o.ptr() + dictoffset);
	}

	template <class O = OBJECT> NODISCARD OBJECT * _get_weaklist_ptr(O const & o)
	{
		TYPE t{ o.type() };
		auto weaklistoffset{ t->tp_weaklistoffset };
		if (weaklistoffset == 0) { return nullptr; }
		VERIFY(0 < weaklistoffset);
		return (OBJECT *)((char *)o.ptr() + weaklistoffset);
	}

	template <class O = OBJECT> NODISCARD OBJECT * _get_vectorcall_ptr(O const & o)
	{
		TYPE t{ o.type() };
		auto vectorcalloffset{ t->tp_vectorcalloffset };
		if (vectorcalloffset == 0) { return nullptr; }
		VERIFY(0 < vectorcalloffset);
		return (OBJECT *)((char *)o.ptr() + vectorcalloffset);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// null
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)		_CoreNull_Type;
	ISM_API_DATA(CoreObject)	_Core_Null;
#define CoreNull_Type			(ism::TYPE{ &ism::_CoreNull_Type })
#define Core_Null				(ism::OBJECT{ &ism::_Core_Null })

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// bool
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreBool_Type;
	ISM_API_DATA(CoreInt)	_Core_True;
	ISM_API_DATA(CoreInt)	_Core_False;
#define CoreBool_Type		(ism::TYPE{ &ism::_CoreBool_Type })
#define Core_True			(ism::INT{ &ism::_Core_True })
#define Core_False			(ism::INT{ &ism::_Core_False })
#define Core_Boolean(b)		((b) ? Core_True : Core_False)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// int
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreInt_Type;
#define CoreInt_Type		(ism::TYPE{ &_CoreInt_Type })

	class NODISCARD ISM_API CoreInt : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreInt;
		using storage_type = ssize_t;

		NODISCARD static auto type_static() { return CoreInt_Type; }

		storage_type m_data;

	public:
		virtual ~CoreInt() override = default;
		CoreInt() : base_type{ type_static() }, m_data{} {}
		CoreInt(self_type const &) = default;
		CoreInt(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
		
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> self_type & operator=(T v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// float
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreFloat_Type;
#define CoreFloat_Type		(ism::TYPE{ &_CoreFloat_Type })

	class NODISCARD ISM_API CoreFloat : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreFloat;
		using storage_type = double_t;

		NODISCARD static auto type_static() { return CoreFloat_Type; }

		storage_type m_data;

	public:
		virtual ~CoreFloat() override = default;
		CoreFloat() : base_type{ type_static() }, m_data{} {}
		CoreFloat(self_type const &) = default;
		CoreFloat(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
		
		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> self_type & operator=(T v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreString_Type;
#define CoreString_Type		(ism::TYPE{ &_CoreString_Type })

	class NODISCARD ISM_API CoreString : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreString;
		using storage_type = String;

		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		NODISCARD static auto type_static() { return CoreString_Type; }

		storage_type m_data;

	public:
		virtual ~CoreString() override = default;
		CoreString() : base_type{ type_static() }, m_data{} {}
		CoreString(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreString(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		CoreString(cstring v, size_t n) : base_type{ type_static() }, m_data{ v, n } {}
		CoreString(cstring v) : base_type{ type_static() }, m_data{ v } {}
		CoreString(self_type const &) = default;
		CoreString(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		template <class O
		> CoreString(ObjectAPI<O> const & o) : self_type{}
		{
			if (ism::isinstance<STR>(o))
			{
				auto s{ const_cast<CoreString *>(static_cast<CoreString const *>(o.handle().ptr())) };
				VERIFY(s);
				m_data = s->m_data;
			}
			else
			{
				m_data = "?";
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto c_str() const noexcept { return m_data.c_str(); }
		NODISCARD auto data() const noexcept { return m_data.data(); }
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	NODISCARD inline OBJECT object_or_cast(cstring s) { return STR::create(s); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// list
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreList_Type;
#define CoreList_Type		(ism::TYPE{ &_CoreList_Type })

	class NODISCARD ISM_API CoreList : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreList;
		using storage_type = Vector<OBJECT>;

		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		NODISCARD static auto type_static() { return CoreList_Type; }

		storage_type m_data;

	public:
		virtual ~CoreList() override = default;
		CoreList() : base_type{ type_static() }, m_data{} {}
		CoreList(std::initializer_list<OBJECT> v) : base_type{ type_static() }, m_data{ v } {}
		CoreList(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreList(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		CoreList(self_type const &) = default;
		CoreList(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		template <class O
		> CoreList(ObjectAPI<O> const & o) : self_type{}
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto data() const noexcept { return m_data.data(); }
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		NODISCARD auto front() const { return m_data.front(); }
		NODISCARD auto back() const { return m_data.back(); }

		template <class O = OBJECT
		> auto append(O && o) { return m_data.emplace_back(ism::object_or_cast(FWD(o))); }

		template <class O = OBJECT
		> auto insert(size_t i, O && o) { return m_data.insert(begin() + i, ism::object_or_cast(FWD(o))); }

		template <class O = OBJECT
		> auto insert(OBJECT i, O && o) { return this->insert(i.cast<size_t>(), FWD(o)); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// dict
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreDict_Type;
#define CoreDict_Type		(ism::TYPE{ &_CoreDict_Type })

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreDict;
		using storage_type = HashMap<OBJECT, OBJECT>;

		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		NODISCARD static auto type_static() { return CoreDict_Type; }

		storage_type m_data;

	public:
		virtual ~CoreDict() override = default;
		CoreDict() : base_type{ type_static() }, m_data{} {}
		CoreDict(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreDict(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		CoreDict(self_type const &) = default;
		CoreDict(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		template <class O
		> CoreDict(ObjectAPI<O> const & o) : self_type{}
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto size() const noexcept { return m_data.size(); }

		template <class K = OBJECT
		> auto find(K && k) const { return m_data.find(object_or_cast(FWD(k))); }

		template <class K = OBJECT
		> bool contains(K && k) const { return this->find(FWD(k)) != end(); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// capsule
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreCapsule_Type;
#define CoreCapsule_Type	(ism::TYPE{ &_CoreCapsule_Type })

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreCapsule;

		NODISCARD static auto type_static() { return CoreCapsule_Type; }

		struct storage_type
		{
			void * ptr{};

			void(*closure)(void *) {};
		}
		m_data;

	public:
		virtual ~CoreCapsule() override { if (m_data.closure) { m_data.closure(m_data.ptr); } }
		CoreCapsule() : base_type{ type_static() }, m_data{} {}
		CoreCapsule(nullptr_t) : base_type{ type_static() }, m_data{} {}
		CoreCapsule(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}
		CoreCapsule(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}
		CoreCapsule(self_type const &) = default;
		CoreCapsule(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		template <class O
		> CoreCapsule(ObjectAPI<O> const & o) : self_type{}
		{
		}

		template <class ... Args
		> CoreCapsule(void const * ptr, Args && ... args) : self_type{ storage_type{ (void *)ptr, FWD(args)... } }
		{
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		template <class T = void
		> NODISCARD auto get_pointer() const -> T * { return static_cast<T *>(m_data.ptr); }

		void set_pointer(void const * value) { m_data.ptr = (void *)value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)	_CoreFunction_Type;
#define CoreFunction_Type	(ism::TYPE{ &_CoreFunction_Type })

	class ISM_API CoreFunction : public CoreObject
	{
	public:
		using base_type = CoreObject;
		using self_type = CoreFunction;

		NODISCARD static auto type_static() { return CoreFunction_Type; }

	public:
		virtual ~CoreFunction() override = default;
		explicit CoreFunction(TYPE const & t) : base_type{ t } {}
		CoreFunction() : base_type{ type_static() } {}
		CoreFunction(self_type const &) = default;
		CoreFunction(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;

		OBJECT cpp_function() const
		{
			OBJECT f{ ism::get_function(handle()) };

			if (f)
			{
				return f;
			}

			return nullptr;
		}

		bool is_cpp_function() const { return cpp_function().is_valid(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPES_HPP_
