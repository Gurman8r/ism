#include <core/api/object/type_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(TypeDB::classes) {};

void TypeDB::add_class(StringName const & name, TypeObject * type)
{
	VERIFY(!name.empty());
	VERIFY(type);
	VERIFY(type->ready());

	hash_t const id{ ism::hash(name) };
	VERIFY(!classes.contains<hash_t>(id));
	classes.push_back(id, name, TYPE(type));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static NumberMethods type_as_number = COMPOSE(NumberMethods, m)
{
};

static SequenceMethods type_as_sequence = COMPOSE(SequenceMethods, m)
{
};

static MappingMethods type_as_mapping = COMPOSE(MappingMethods, m)
{
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(TypeObject, t)
{
	t.tp_name = "type";
	t.tp_size = sizeof(TypeObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_Type_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(TypeObject, tp_dict);
	t.tp_weaklist_offset = offsetof(TypeObject, tp_weaklist);
	t.tp_vectorcall_offset = offsetof(TypeObject, tp_vectorcall);

	t.tp_hash = (hashfunc)[](OBJECT o) { return ism::hash(TYPE(o)->tp_name); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };

	t.tp_as_number = &type_as_number;
	t.tp_as_sequence = &type_as_sequence;
	t.tp_as_mapping = &type_as_mapping;

	t.tp_getattro = (getattrofunc)type_getattr;
	t.tp_setattro = (setattrofunc)type_setattr;

	t.tp_finalize = (destructor)[](BaseObject * ptr)
	{
		if (TypeObject * t{ super_cast<TypeObject>(ptr) })
		{
			t->tp_bases = nullptr;
			t->tp_cache = nullptr;
			t->tp_dict = nullptr;
			t->tp_mro = nullptr;
			t->tp_subclasses = nullptr;
			t->tp_dict = nullptr;
			t->tp_weaklist = nullptr;
		}
	};

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };

	t.tp_free = (freefunc)[](void * ptr) { memdelete((TypeObject *)ptr); };

	t.tp_init = (initproc)type_init;

	t.tp_new = (newfunc)type_new;

	t.tp_call = (binaryfunc)type_call;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void TypeObject::_bind_class(TypeObject & t)
{
	CLASS_<TYPE>(&t, "type")

		.def("__contains__", [](TYPE self, OBJECT value)
		{
			return DICT(self->tp_dict).contains(value);
		})

		.def("__instancecheck__", [](OBJECT inst, OBJECT cls)
		{
			return isinstance(inst, cls);
		})

		.def("__subclasscheck__", [](OBJECT derived, OBJECT cls)
		{
			if (TYPE::check_(cls) && TYPE::check_(derived)) {
				return TYPE(derived).is_subtype(TYPE(cls));
			}
			return false;
		})

		.def_property_readonly("__size__", [](TYPE self) { return self->tp_size; })
	
		.def_property_readonly("__flags__", [](TYPE self) { return self->tp_flags; })
	
		.def_property_readonly("__base__", [](TYPE self) { return TYPE(self->tp_base); })
	
		.def_property_readonly("__mro__", [](TYPE self) { return self->tp_mro; })
	
		.def_property_readonly("__dict__", [](TYPE self) { return self->tp_dict; })
	
		.def_property_readonly("__weaklist__", [](TYPE self) { return self->tp_weaklist; })
	
		.def_property_readonly("__text_signature__", [](TYPE self) { return STR{}; })
	
		.def_property_readonly("__qualname__", [](TYPE self) { return STR{}; })
	
		.def_property("__name__", [](TYPE self) { return self->tp_name; }, [](TYPE self, STR value) { self->tp_name = value; })
	
		.def_property("__doc__", [](TYPE self) { return self->tp_doc; }, [](TYPE self, STR value) { self->tp_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::ready()
{
	if (has_feature(TypeFlags_Ready)) { VERIFY(check_consistency()); return true; }
	
	VERIFY(!has_feature(TypeFlags_Readying));

	tp_flags |= TypeFlags_Readying;

	if (!tp_base && this != *typeof<OBJECT>()) { tp_base = typeof<OBJECT>(); }
	
	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }
	
	if (!ob_type && tp_base) { ob_type = tp_base->ob_type; }
	
	tp_bases = LIST(tp_base ? ListObject{ tp_base } : ListObject{});
	
	if (!tp_dict) { tp_dict = DICT(DictObject{}); }
	
	VERIFY(mro_internal(nullptr));
	
	if (tp_base) { inherit_special(*tp_base); }

	for (TYPE const & base : LIST(tp_mro)) { inherit_slots(*base); }

	if (has_feature(TypeFlags_HaveGc | TypeFlags_BaseType) && (!tp_free || tp_free == memfree))
	{
		VERIFY(!"type participates in gc and is a base type but has inappropriate tp_free slot");
	}

	copy_val(*tp_base, &TypeObject::tp_as_number);
	copy_val(*tp_base, &TypeObject::tp_as_sequence);
	copy_val(*tp_base, &TypeObject::tp_as_mapping);

	for (TYPE const & base : LIST(tp_bases)) { base->add_subclass(this); }

	tp_flags = (tp_flags & ~TypeFlags_Readying) | TypeFlags_Ready;

	return true;
}

bool TypeObject::check_consistency() const
{
	// don't check static types before ready()
	if (!has_feature(TypeFlags_Ready)) { return true; }

	VERIFY(TYPE::check_(handle()));

	VERIFY(!has_feature(TypeFlags_Readying));

	VERIFY(tp_dict);

	return true;
}

OBJECT TypeObject::lookup(OBJECT const & name) const
{
	if (LIST mro; !name || !(mro = tp_mro)) { return nullptr; }
	else
	{
		for (TYPE const & base : mro)
		{
			if (OBJECT * item{ DICT(base->tp_dict).lookup(name) })
			{
				return (*item);
			}
		}
		return nullptr;
	}
}

bool TypeObject::is_subtype(TYPE const & value) const
{
	if (LIST mro{ tp_mro })
	{
		for (TYPE const & base : mro) { if (value.is(base)) { return true; } }

		return false;
	}
	else
	{
		TypeObject const * base{ this };

		do {

			if (value.is(base)) { return true; }

			base = *base->tp_base;

		} while (base);

		return value.is(typeof<OBJECT>());
	}
}

Error TypeObject::update_slot(OBJECT name)
{
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::add_subclass(TypeObject * type)
{
	if (!tp_subclasses) { tp_subclasses = DICT(DictObject{}); }

	(DICT(tp_subclasses))[type] = OBJECT(type);

	return true;
}

bool TypeObject::mro_internal(OBJECT * in_old_mro)
{
	OBJECT old_mro{ tp_mro };

	OBJECT new_mro{ std::invoke([&, &bases = LIST(tp_bases)]()->OBJECT
	{
		// mro_implementation
		LIST result{ ListObject{} };
		result.reserve(bases.size() + 1);
		result.append(this);
		for (TYPE const & base : bases) { result.append(base); }
		return result;
	}) };
	
	bool const reent{ tp_mro != old_mro };
	
	if (!new_mro) { return false; }
	
	if (reent) { return true; }
	
	tp_mro = new_mro;
	
	if (in_old_mro) { *in_old_mro = old_mro; }
	
	return true;
}

void TypeObject::inherit_special(TypeObject * base)
{
	if (!has_feature(TypeFlags_HaveGc) && flag_read(base->tp_flags, TypeFlags_HaveGc) && (!tp_traverse && !tp_clear))
	{
		flag_set(tp_flags, TypeFlags_HaveGc);
		tp_traverse = base->tp_traverse;
		tp_clear = base->tp_clear;
	}

	if (base != *typeof<OBJECT>() || has_feature(TypeFlags_HeapType) && !tp_new)
	{
		tp_new = base->tp_new;
	}

	copy_val(base, &TypeObject::tp_size);
	copy_val(base, &TypeObject::tp_weaklist_offset);
	copy_val(base, &TypeObject::tp_dict_offset);

	//if (LIST mro{ base->tp_mro })
	//{
	//	if (mro.contains(typeof<TYPE>())) { flag_set(tp_flags, TypeFlags_Type_Subclass); }
	//	else if (mro.contains(typeof<INT>())) { flag_set(tp_flags, TypeFlags_Int_Subclass); }
	//	else if (mro.contains(typeof<FLT>())) { flag_set(tp_flags, TypeFlags_Float_Subclass); }
	//	else if (mro.contains(typeof<STR>())) { flag_set(tp_flags, TypeFlags_Str_Subclass); }
	//	else if (mro.contains(typeof<LIST>())) { flag_set(tp_flags, TypeFlags_List_Subclass); }
	//	else if (mro.contains(typeof<DICT>())) { flag_set(tp_flags, TypeFlags_Dict_Subclass); }
	//}
}

void TypeObject::inherit_slots(TypeObject * base)
{
	TypeObject * basebase{};

	if (tp_as_number && base->tp_as_number)
	{
		if (basebase = *(base->tp_base); !basebase->tp_as_number) { basebase = nullptr; }
		copy_slot(base, basebase, &NumberMethods::operator_add);
		copy_slot(base, basebase, &NumberMethods::operator_subtract);
		copy_slot(base, basebase, &NumberMethods::operator_multiply);
		copy_slot(base, basebase, &NumberMethods::operator_divide);
		copy_slot(base, basebase, &NumberMethods::operator_remainder);
		copy_slot(base, basebase, &NumberMethods::operator_power);
		copy_slot(base, basebase, &NumberMethods::operator_lshift);
		copy_slot(base, basebase, &NumberMethods::operator_rshift);
		copy_slot(base, basebase, &NumberMethods::operator_and);
		copy_slot(base, basebase, &NumberMethods::operator_xor);
		copy_slot(base, basebase, &NumberMethods::operator_or);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_add);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_subtract);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_multiply);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_divide);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_remainder);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_power);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_lshift);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_rshift);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_and);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_xor);
		copy_slot(base, basebase, &NumberMethods::operator_inplace_or);
		copy_slot(base, basebase, &NumberMethods::operator_positive);
		copy_slot(base, basebase, &NumberMethods::operator_negative);
		copy_slot(base, basebase, &NumberMethods::operator_absolute);
		copy_slot(base, basebase, &NumberMethods::operator_bool);
		copy_slot(base, basebase, &NumberMethods::operator_invert);
		copy_slot(base, basebase, &NumberMethods::operator_integer);
		copy_slot(base, basebase, &NumberMethods::operator_decimal);
	}

	if (tp_as_mapping && base->tp_as_mapping)
	{
		if (basebase = *(base->tp_base); !basebase->tp_as_mapping) { basebase = nullptr; }
		copy_slot(base, basebase, &SequenceMethods::sequence_length);
		copy_slot(base, basebase, &SequenceMethods::sequence_item);
		copy_slot(base, basebase, &SequenceMethods::sequence_contains);
	}

	if (tp_as_sequence && base->tp_as_sequence)
	{
		if (basebase = *(base->tp_base); !basebase->tp_as_sequence) { basebase = nullptr; }
		copy_slot(base, basebase, &MappingMethods::mapping_length);
		copy_slot(base, basebase, &MappingMethods::mapping_subscript);
	}

	basebase = *(base->tp_base);

	copy_slot(base, basebase, &TypeObject::tp_dealloc);

	if (!tp_getattr && !tp_getattro) { tp_getattr = base->tp_getattr; tp_getattro = base->tp_getattro; }

	if (!tp_setattr && !tp_setattro) { tp_setattr = base->tp_setattr; tp_setattro = base->tp_setattro; }

	copy_slot(base, basebase, &TypeObject::tp_len);
	copy_slot(base, basebase, &TypeObject::tp_repr);
	copy_slot(base, basebase, &TypeObject::tp_str);

	copy_slot(base, basebase, &TypeObject::tp_vectorcall_offset);
	if (!tp_call && flag_read(base->tp_flags, TypeFlags_HaveVectorCall) && !has_feature(TypeFlags_HeapType))
	{
		flag_set(tp_flags, TypeFlags_HaveVectorCall);
	}
	copy_slot(base, basebase, &TypeObject::tp_call);

	if (!tp_compare && !tp_hash && !tp_dict->contains("__eq__") && !tp_dict->contains("__hash__"))
	{
		tp_compare = base->tp_compare;
		tp_hash = base->tp_hash;
	}

	copy_slot(base, basebase, &TypeObject::tp_descr_get);
	if (base->tp_descr_get && tp_descr_get == base->tp_descr_get && !has_feature(TypeFlags_HeapType) && flag_read(base->tp_flags, TypeFlags_MethodDescriptor))
	{
		flag_set(tp_flags, TypeFlags_MethodDescriptor);
	}
	copy_slot(base, basebase, &TypeObject::tp_descr_set);

	copy_slot(base, basebase, &TypeObject::tp_dict_offset);
	copy_slot(base, basebase, &TypeObject::tp_init);
	copy_slot(base, basebase, &TypeObject::tp_alloc);
	copy_slot(base, basebase, &TypeObject::tp_is_gc);
	copy_slot(base, basebase, &TypeObject::tp_finalize);
	copy_slot(base, basebase, &TypeObject::tp_free);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::api::type_getattr(TYPE type, OBJECT name)
{
	if (!type->tp_dict) { if (!type->ready()) { return nullptr; } }

	TYPE metatype{ typeof(type) };

	OBJECT meta_attribute{ metatype.lookup(name) };

	descrgetfunc meta_get{};

	if (TYPE atype; meta_attribute && (atype = typeof(meta_attribute)))
	{
		if ((meta_get = atype->tp_descr_get) && atype->tp_descr_set)
		{
			return meta_get(meta_attribute, type, metatype);
		}
	}

	if (OBJECT attribute{ type.lookup(name) })
	{
		if (descrgetfunc local_get{ typeof(attribute)->tp_descr_get })
		{
			return local_get(attribute, nullptr, type);
		}
	}

	if (meta_get) { return meta_get(meta_attribute, type, metatype); }

	if (meta_attribute) { return meta_attribute; }

	return nullptr;
}

Error ism::api::type_setattr(TYPE type, OBJECT name, OBJECT value)
{
	Error err{ generic_setattr(type, name, value) };

	if (err == Error_None)
	{
		err = type->update_slot(name);

		VERIFY(type->check_consistency());
	}

	return err;
}

OBJECT ism::api::type_call(OBJECT self, OBJECT args)
{
	TYPE type{ self }; VERIFY(type);

	// type(x) should return typeof(x)
	if (type.is(typeof<TYPE>()))
	{
		if (ssize_t const nargs{ len(args) }; nargs == 1)
		{
			return typeof(LIST(args)[0]);
		}
		else
		{
			return nullptr;
		}
	}

	if (!type->tp_new) { return nullptr; }

	OBJECT obj{ type->tp_new(type, args) };

	if (!obj) { return nullptr; }

	if (!typeof(obj).is_subtype(type)) { return obj; }

	type = typeof(obj);

	if (type->tp_init)
	{
		Error const err{ type->tp_init(obj, args) };

		VERIFY(err == Error_None);
	}

	return obj;
}

Error ism::api::type_init(OBJECT self, OBJECT args)
{
	return object_init(self, args);
}

OBJECT ism::api::type_new(TYPE type, OBJECT args)
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */