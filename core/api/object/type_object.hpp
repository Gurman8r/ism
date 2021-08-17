#ifndef _ISM_TYPE_OBJECT_HPP_
#define _ISM_TYPE_OBJECT_HPP_

#include <core/api/object/base_object.hpp>

// typedb
namespace ism
{
	class ISM_API TypeDB
	{
	public:
		static Batch<
			hash_t,
			StringName,
			TYPE
		> classes;

	public:
		static void add_class(StringName const & name, TYPE type);

		template <class T
		> static void add_class() { add_class(ctti::type_v<T>, T::get_class()); }

		template <class T
		> static void bind_class(OBJ scope)
		{
			T::initialize_class(scope);

			TYPE t{ *classes.map<hash_t, TYPE>(hash(ctti::type_v<T>)) };

			if (t.is_null())
			{
				FATAL("failed to register class");
			}
		}
	};
}

// type
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type object
	class ISM_API TypeObject : public Object
	{
		ISM_OBJECT_COMMON(TypeObject, Object);

	protected:
		static void _bind_class(OBJ scope);

		virtual TYPE _get_typev() const noexcept override;

	public:
		explicit TypeObject(TYPE const & t) noexcept;

		TypeObject() noexcept;

		NODISCARD static TYPE get_class() noexcept;

	public:
		String				tp_name{};
		ssize_t				tp_size{};
		int32_t				tp_flags{};
		String				tp_doc{};

		ssize_t				tp_dictoffset{};
		ssize_t				tp_weaklistoffset{};
		ssize_t				tp_vectorcalloffset{};

		binaryfunc			tp_call{};
		inquiry				tp_clear{};
		cmpfunc				tp_compare{};
		hashfunc			tp_hash{};
		lenfunc				tp_len{};
		reprfunc			tp_repr{};
		reprfunc			tp_str{};
		traverseproc		tp_traverse{};

		getattrfunc			tp_getattr{};
		setattrfunc			tp_setattr{};
		getattrofunc		tp_getattro{};
		setattrofunc		tp_setattro{};
		descrgetfunc		tp_descr_get{};
		descrsetfunc		tp_descr_set{};

		destructor			tp_dealloc{};
		initproc			tp_init{};
		allocfunc			tp_alloc{};
		newfunc				tp_new{};
		freefunc			tp_free{};
		destructor			tp_finalize{};

		NumberMethods *		tp_as_number{};
		SequenceMethods *	tp_as_sequence{};
		MappingMethods *	tp_as_mapping{};

		Ref<TypeObject>		tp_base{ /* Handle<TypeObject> doesn't exist yet */ };
		OBJ					tp_bases{};
		OBJ					tp_cache{};
		OBJ					tp_dict{};
		OBJ					tp_mro{};
		OBJ					tp_subclasses{};
		vectorcallfunc		tp_vectorcall{};

	public:
		NODISCARD bool ready();

		NODISCARD bool check_consistency() const;

		NODISCARD OBJ lookup(OBJ const & name) const;

		NODISCARD bool is_subtype(TYPE const & value) const;

		NODISCARD Error update_slot(OBJ name);

		NODISCARD bool has_feature(int32_t flag) const { return flag_read(tp_flags, flag); }

	protected:
		bool add_subclass(TypeObject * type);

		bool mro_internal(OBJ * old_mro);

		void inherit_special(TypeObject * base);

		void inherit_slots(TypeObject * base);

	public:
		template <class Slot> bool slot_defined(TypeObject * base, Slot TypeObject:: * slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> bool slot_defined(TypeObject * base, Slot NumberMethods:: * slot) const
		{
			return ((*this->tp_as_number).*slot) && (!base || ((*this->tp_as_number).*slot) != ((*base->tp_as_number).*slot));
		}

		template <class Slot> bool slot_defined(TypeObject * base, Slot SequenceMethods:: * slot) const
		{
			return ((*this->tp_as_sequence).*slot) && (!base || ((*this->tp_as_sequence).*slot) != ((*base->tp_as_sequence).*slot));
		}

		template <class Slot> bool slot_defined(TypeObject * base, Slot MappingMethods:: * slot) const
		{
			return ((*this->tp_as_mapping).*slot) && (!base || ((*this->tp_as_mapping).*slot) != ((*base->tp_as_mapping).*slot));
		}

		template <class Slot> void copy_val(TypeObject * base, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base)
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot NumberMethods:: * slot)
		{
			if (!((*this->tp_as_number).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_number).*slot) = ((*base->tp_as_number).*slot);
			}
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot SequenceMethods:: * slot)
		{
			if (!((*this->tp_as_sequence).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_sequence).*slot) = ((*base->tp_as_sequence).*slot);
			}
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot MappingMethods:: * slot)
		{
			if (!((*this->tp_as_mapping).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_mapping).*slot) = ((*base->tp_as_mapping).*slot);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type delete
	template <> struct DefaultDelete<TypeObject>
	{
		void operator()(TypeObject * ptr) const { memdelete(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type check
#define ISM_CHECK_TYPE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Type_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type handle
	template <> class Handle<TypeObject> : public Ref<TypeObject>
	{
		ISM_HANDLE_DEFAULT(TypeObject, ISM_CHECK_TYPE);

	public:
		NODISCARD bool ready() const { return m_ptr->ready(); }

		NODISCARD bool has_feature(int32_t flag) const { return m_ptr->has_feature(flag); }

		NODISCARD OBJ lookup(OBJ const & name) const { return m_ptr->lookup(name); }

		NODISCARD bool is_subtype(TYPE const & value) const { return m_ptr->is_subtype(value); }

		NODISCARD auto name() const { return attr("__name__"); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD hash_t hash(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_hash ? t->tp_hash(o) : 0;
	}

	template <class T> NODISCARD ssize_t len(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_len ? t->tp_len(o) : -1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_dict_ptr(TYPE const & t, OBJ const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_dictoffset : 0 }; 0 < offset)
		{
			return reinterpret_cast<OBJ *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (OBJ *)nullptr;
		}
	}

	NODISCARD inline auto get_dict_ptr(OBJ const & o) noexcept { return get_dict_ptr(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_weaklist_ptr(TYPE const & t, OBJ const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_weaklistoffset : 0 }; 0 < offset)
		{
			return reinterpret_cast<OBJ *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (OBJ *)nullptr;
		}
	}

	NODISCARD inline auto get_weaklist_ptr(OBJ const & o) noexcept { return get_weaklist_ptr(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_vectorcall_func(TYPE const & t, OBJ const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_vectorcalloffset : 0 }; 0 < offset)
		{
			return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (vectorcallfunc)nullptr;
		}
	}

	NODISCARD inline auto get_vectorcall_func(OBJ const & o) noexcept { return get_vectorcall_func(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) type_getattr(TYPE type, OBJ name);

	ISM_API_FUNC(Error) type_setattr(TYPE type, OBJ name, OBJ value);

	ISM_API_FUNC(OBJ) type_call(OBJ self, OBJ args);

	ISM_API_FUNC(Error) type_init(OBJ self, OBJ args);
	
	ISM_API_FUNC(OBJ) type_new(TYPE type, OBJ args);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_OBJECT_HPP_
