#ifndef _ISM_TYPE_OBJECT_HPP_
#define _ISM_TYPE_OBJECT_HPP_

#include <core/api/object/base_object.hpp>

// type object
class NODISCARD ISM_API ism::api::TypeObject : public BaseObject
{
	ISM_OBJECT(TypeObject, BaseObject);

protected:
	static void _bind_class(ism::api::TypeObject & t);

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
	MappingMethods  *	tp_as_mapping{};

	Ref<TypeObject>		tp_base{ /* Handle<TypeObject> doesn't exist yet */ };
	OBJECT				tp_bases{};
	OBJECT				tp_cache{};
	OBJECT				tp_dict{};
	OBJECT				tp_mro{};
	OBJECT				tp_subclasses{};
	OBJECT				tp_weaklist{};

public:
	NODISCARD bool has_feature(int32_t feature) const noexcept { return flag_read(tp_flags, feature); }

	void enable_feature(int32_t feature) noexcept { flag_set(tp_flags, feature); }

	void disable_feature(int32_t feature) noexcept { flag_clear(tp_flags, feature); }

public:
	bool ready();

	bool add_subclass(TypeObject * type);

	bool mro_internal(OBJECT * old_mro);

	void inherit_special(TypeObject * base);

	void inherit_slots(TypeObject * base);

public:
	void modified();

	Error update_slot(OBJECT name);

	bool check_consistency() const;

public:
	template <class Slot> bool slot_defined(TypeObject const * base, Slot TypeObject::*slot) const
	{
		return (this->*slot) && (!base || (this->*slot) != (base->*slot));
	}

	template <class Slot> bool slot_defined(TypeObject const * base, Slot NumberMethods::*slot) const
	{
		return ((*this->tp_as_number).*slot) && (!base || ((*this->tp_as_number).*slot) != ((*base->tp_as_number).*slot));
	}

	template <class Slot> bool slot_defined(TypeObject const * base, Slot SequenceMethods::*slot) const
	{
		return ((*this->tp_as_sequence).*slot) && (!base || ((*this->tp_as_sequence).*slot) != ((*base->tp_as_sequence).*slot));
	}

	template <class Slot> bool slot_defined(TypeObject const * base, Slot MappingMethods::*slot) const
	{
		return ((*this->tp_as_mapping).*slot) && (!base || ((*this->tp_as_mapping).*slot) != ((*base->tp_as_mapping).*slot));
	}

	template <class Slot> void copy_val(TypeObject const * base, Slot TypeObject::*slot)
	{
		if (!(this->*slot) && base)
		{
			(this->*slot) = (base->*slot);
		}
	}

	template <class Slot> auto copy_slot(TypeObject const * base, TypeObject const * basebase, Slot TypeObject::*slot)
	{
		if (!(this->*slot) && base && base->slot_defined(basebase, slot))
		{
			(this->*slot) = (base->*slot);
		}
	}

	template <class Slot> auto copy_slot(TypeObject const * base, TypeObject const * basebase, Slot NumberMethods::*slot)
	{
		if (!((*this->tp_as_number).*slot) && base && base->slot_defined(basebase, slot))
		{
			((*this->tp_as_number).*slot) = ((*base->tp_as_number).*slot);
		}
	}

	template <class Slot> auto copy_slot(TypeObject const * base, TypeObject const * basebase, Slot SequenceMethods::*slot)
	{
		if (!((*this->tp_as_sequence).*slot) && base && base->slot_defined(basebase, slot))
		{
			((*this->tp_as_sequence).*slot) = ((*base->tp_as_sequence).*slot);
		}
	}

	template <class Slot> auto copy_slot(TypeObject const * base, TypeObject const * basebase, Slot MappingMethods::*slot)
	{
		if (!((*this->tp_as_mapping).*slot) && base && base->slot_defined(basebase, slot))
		{
			((*this->tp_as_mapping).*slot) = ((*base->tp_as_mapping).*slot);
		}
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// type deleter
template <> struct ism::DefaultDelete<ism::api::TypeObject>
{
	void operator()(api::TypeObject * ptr) const { memdelete(ptr); }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// type handle
template <> class ism::api::Handle<ism::api::TypeObject> : public ism::api::BaseHandle<ism::api::TypeObject>
{
	ISM_HANDLE(TypeObject);

public:
	Handle() = default;

	~Handle() = default;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// functions
namespace ism::api
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

	NODISCARD inline auto get_dict_ptr(TYPE const & t, OBJECT const & o)
	{
		if (o && t && 0 < t->tp_dict_offset)
		{
			return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(*o) + t->tp_dict_offset);
		}
		else
		{
			return (OBJECT *)nullptr;
		}
	}

	NODISCARD inline auto get_dict_ptr(OBJECT const & o) noexcept
	{
		return get_dict_ptr(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_weaklist_ptr(TYPE const & t, OBJECT const & o)
	{
		if (o && t && 0 < t->tp_weaklist_offset)
		{
			return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(*o) + t->tp_weaklist_offset);
		}
		else
		{
			return (OBJECT *)nullptr;
		}
	}

	NODISCARD inline auto get_weaklist_ptr(OBJECT const & o) noexcept
	{
		return get_weaklist_ptr(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_vectorcall_func(TYPE const & t, OBJECT const & o)
	{
		if (o && t && 0 < t->tp_vectorcall_offset)
		{
			return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(*o) + t->tp_vectorcall_offset);
		}
		else
		{
			return (vectorcallfunc)nullptr;
		}
	}

	NODISCARD inline auto get_vectorcall_func(OBJECT const & o) noexcept
	{
		return get_vectorcall_func(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJECT) find_name_in_mro(TYPE type, OBJECT name, Error * err);

	ISM_API_FUNC(OBJECT) type_lookup(TYPE type, OBJECT name);

	ISM_API_FUNC(OBJECT) type_getattro(TYPE type, OBJECT name);

	ISM_API_FUNC(Error) type_setattro(TYPE type, OBJECT name, OBJECT value);

	ISM_API_FUNC(OBJECT) generic_getattr_with_dict(OBJECT obj, OBJECT name, OBJECT dict);

	ISM_API_FUNC(Error) generic_setattr_with_dict(OBJECT obj, OBJECT name, OBJECT value, OBJECT dict);
	
	inline OBJECT generic_getattr(OBJECT obj, OBJECT name) noexcept { return generic_getattr_with_dict(obj, name, nullptr); }

	inline Error generic_setattr(OBJECT obj, OBJECT name, OBJECT value) noexcept { return generic_setattr_with_dict(obj, name, value, nullptr); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_OBJECT_HPP_
