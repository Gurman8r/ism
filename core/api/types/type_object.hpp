#ifndef _ISM_API_TYPES_TYPE_OBJECT_HPP_
#define _ISM_API_TYPES_TYPE_OBJECT_HPP_

#include <core/api/types/base_object.hpp>

// type
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreType : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreType, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		StringName			tp_name{};
		ssize_t				tp_basicsize{}, tp_itemsize{};
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
		descrgetfunc		tp_descrget{};
		descrsetfunc		tp_descrset{};

		destructor			tp_delete{};
		initproc			tp_init{};
		allocfunc			tp_alloc{};
		newfunc				tp_new{};
		freefunc			tp_free{};
		inquiry				tp_is_gc{};
		destructor			tp_finalize{};

		GetSetDef *			tp_getsets{};
		NumberMethods *		tp_as_number{};
		SequenceMethods *	tp_as_sequence{};
		MappingMethods  *	tp_as_mapping{};

		TYPE				tp_base{};
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

		bool ready();
		bool add_subclass(TYPE const & type);
		bool mro_internal(OBJECT * old_mro);
		void inherit_special(CoreType const * base);
		void inherit_slots(CoreType const * base);

	public:
		template <class Slot> bool slot_defined(CoreType const * base, Slot CoreType::*slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot NumberMethods::*slot) const
		{
			return ((*this->tp_as_number).*slot) && (!base || ((*this->tp_as_number).*slot) != ((*base->tp_as_number).*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot SequenceMethods::*slot) const
		{
			return ((*this->tp_as_sequence).*slot) && (!base || ((*this->tp_as_sequence).*slot) != ((*base->tp_as_sequence).*slot));
		}

		template <class Slot> bool slot_defined(CoreType const * base, Slot MappingMethods::*slot) const
		{
			return ((*this->tp_as_mapping).*slot) && (!base || ((*this->tp_as_mapping).*slot) != ((*base->tp_as_mapping).*slot));
		}

	public:
		template <class Slot> void copy_val(CoreType const * base, Slot CoreType::*slot)
		{
			if (!(this->*slot) && base)
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot CoreType::*slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot NumberMethods::*slot)
		{
			if (!((*this->tp_as_number).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_number).*slot) = ((*base->tp_as_number).*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot SequenceMethods::*slot)
		{
			if (!((*this->tp_as_sequence).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_sequence).*slot) = ((*base->tp_as_sequence).*slot);
			}
		}

		template <class Slot> auto copy_slot(CoreType const * base, CoreType const * basebase, Slot MappingMethods::*slot)
		{
			if (!((*this->tp_as_mapping).*slot) && base && base->slot_defined(basebase, slot))
			{
				((*this->tp_as_mapping).*slot) = ((*base->tp_as_mapping).*slot);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		NODISCARD inline GetSetDef * get_property_def(OBJECT o, cstring name)
		{
			if (hash_t id{ hash(name, strlen(name)) }; TYPE type{ typeof(o) })
			{
				for (GetSetDef * def{ type->tp_getsets }; def && *def; ++def)
				{
					if (id == hash(def->name, strlen(def->name)))
					{
						return def;
					}
				}
			}
			return nullptr;
		}

		NODISCARD inline auto get_dict_ptr(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && 0 < t->tp_dict_offset)
			{
				return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(o.ptr()) + t->tp_dict_offset);
			}
			else
			{
				return (OBJECT *)nullptr;
			}
		}

		NODISCARD inline auto get_weaklist_ptr(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && 0 < t->tp_weaklist_offset)
			{
				return reinterpret_cast<OBJECT *>(reinterpret_cast<char *>(o.ptr()) + t->tp_weaklist_offset);
			}
			else
			{
				return (OBJECT *)nullptr;
			}
		}

		NODISCARD inline auto get_vectorcall_func(OBJECT const & o)
		{
			if (TYPE t{ typeof(o) }; t && t->has_feature(TypeFlags_HaveVectorCall) && 0 < t->tp_vectorcall_offset)
			{
				return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(o.ptr()) + t->tp_vectorcall_offset);
			}
			else
			{
				return (vectorcallfunc)nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_TYPE_OBJECT_HPP_
