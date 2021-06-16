#ifndef _ISM_TYPE_OBJECT_HPP_
#define _ISM_TYPE_OBJECT_HPP_

#include <core/api/class_db.hpp>

// type object
namespace ism::api
{
	class ISM_API TypeObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(TypeObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		String				tp_name{};
		ssize_t				tp_size{};
		int32_t				tp_flags{};
		String				tp_doc{};

		ssize_t				tp_dict_offset{};
		ssize_t				tp_weaklist_offset{};
		ssize_t				tp_vectorcall_offset{};

		binaryfunc			tp_call{};
		inquiry				tp_clear{};
		cmpfunc				tp_compare{};
		hashfunc			tp_hash{};
		lenfunc				tp_len{};
		reprfunc			tp_repr{};
		reprfunc			tp_str{};
		traverseproc		tp_traverse{};
		vectorcallfunc		tp_vectorcall{};

		getattrfunc			tp_getattr{};
		setattrfunc			tp_setattr{};
		getattrofunc		tp_getattro{};
		setattrofunc		tp_setattro{};
		descrgetfunc		tp_descr_get{};
		descrsetfunc		tp_descr_set{};

		destructor			tp_delete{};
		initproc			tp_init{};
		allocfunc			tp_alloc{};
		newfunc				tp_new{};
		freefunc			tp_free{};
		inquiry				tp_is_gc{};
		destructor			tp_finalize{};

		NumberMethods *		tp_as_number{};
		SequenceMethods *	tp_as_sequence{};
		MappingMethods *	tp_as_mapping{};

		Ref<TypeObject>		tp_base{ /* Handle<TypeObject> doesn't exist yet */ };
		OBJECT				tp_bases{};
		OBJECT				tp_cache{};
		OBJECT				tp_dict{};
		OBJECT				tp_mro{};
		OBJECT				tp_subclasses{};
		OBJECT				tp_weaklist{};

	public:
		bool ready();

		bool add_subclass(TypeObject * type);

		bool mro_internal(OBJECT * old_mro);

		void inherit_special(TypeObject * base);

		void inherit_slots(TypeObject * base);

		Error update_slot(OBJECT name);

		bool check_consistency() const;

		OBJECT lookup(OBJECT name) const;

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
}

// type deleter
namespace ism
{
	template <> struct DefaultDelete<api::TypeObject>
	{
		void operator()(api::TypeObject * ptr) const { memdelete(ptr); }
	};
}

// type handle
namespace ism::api
{
	template <> class Handle<TypeObject> : public BaseHandle<TypeObject>
	{
		ISM_HANDLE_DEFAULT(TypeObject);

	public:
		Handle() = default;

		~Handle() = default;
	};
}

// functions
namespace ism::api
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJECT) type_getattro(TYPE type, OBJECT name);

	ISM_API_FUNC(Error) type_setattro(TYPE type, OBJECT name, OBJECT value);

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

	NODISCARD inline auto get_dict_ptr(TYPE const & t, OBJECT const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_dict_offset : 0 }; 0 < offset)
		{
			return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (OBJECT *)nullptr;
		}
	}

	NODISCARD inline auto get_dict_ptr(OBJECT const & o) noexcept { return get_dict_ptr(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_weaklist_ptr(TYPE const & t, OBJECT const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_weaklist_offset : 0 }; 0 < offset)
		{
			return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (OBJECT *)nullptr;
		}
	}

	NODISCARD inline auto get_weaklist_ptr(OBJECT const & o) noexcept { return get_weaklist_ptr(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_vectorcall_func(TYPE const & t, OBJECT const & o)
	{
		if (ssize_t const offset{ (o && t) ? t->tp_vectorcall_offset : 0 }; 0 < offset)
		{
			return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(*o) + offset);
		}
		else
		{
			return (vectorcallfunc)nullptr;
		}
	}

	NODISCARD inline auto get_vectorcall_func(OBJECT const & o) noexcept { return get_vectorcall_func(typeof(o), o); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_OBJECT_HPP_
