#include <core/api/object/type_object.hpp>
#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

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

ISM_BUILTIN_TYPE(TypeObject, t)
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

	t.tp_getattro = (getattrofunc)type_getattro;
	t.tp_setattro = (setattrofunc)type_setattro;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((TypeObject *)ptr); };

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

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<TYPE>(v))
		{
			return util::compare(TYPE(o)->tp_name, TYPE(v)->tp_name);
		}
		else
		{
			return util::compare(*o, *v);
		}
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void TypeObject::_bind_methods(TypeObject & t)
{
	t.tp_dict["__contains__"] = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
		return DICT(self->tp_dict).contains(value);
	});

	t.tp_dict["__dir__"] = CPP_FUNCTION([](TYPE self)
	{
	});

	t.tp_dict["__instancecheck__"] = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
	});

	t.tp_dict["mro"] = CPP_FUNCTION([](TYPE self)
	{
	});

	t.tp_dict["__sizeof__"] = CPP_FUNCTION([](TYPE self)
	{
	});

	t.tp_dict["__subclasscheck__"] = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
	});

	t.tp_dict["__subclasses__"] = CPP_FUNCTION([](TYPE self)
	{
	});

	t.tp_dict["__basicsize__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_size; })
		});
	
	t.tp_dict["__flags__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_flags; })
		});
	
	t.tp_dict["__base__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return TYPE(self->tp_base); })
		});
	
	t.tp_dict["__mro__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_mro; })
		});
	
	t.tp_dict["__dict__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_dict; })
		});
	
	t.tp_dict["__weaklist__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_weaklist; })
		});
	
	t.tp_dict["__text_signature__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return String{}; })
		});
	
	t.tp_dict["__qualname__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return String{}; })
		});

	t.tp_dict["__bases__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_bases; }),
		CPP_FUNCTION([](TYPE self, OBJECT value) { self->tp_bases = value; }),
		});

	t.tp_dict["__module__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_dict["__module__"]; }),
		CPP_FUNCTION([](TYPE self, OBJECT value) { self->tp_dict["__module__"] = value;  })
		});

	t.tp_dict["__abstractmethods__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_dict["__abstractmethods__"]; }),
		CPP_FUNCTION([](TYPE self, OBJECT value) { self->tp_dict["__abstractmethods__"] = value;  })
		});

	t.tp_dict["__name__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_name; }),
		CPP_FUNCTION([](TYPE self, STR value) { self->tp_name = value; })
		});

	t.tp_dict["__doc__"] = PROPERTY({
		CPP_FUNCTION([](TYPE self) { return self->tp_doc; }),
		CPP_FUNCTION([](TYPE self, STR value) { self->tp_doc = value; })
		});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::ready()
{
	if (flag_read(tp_flags, TypeFlags_Ready)) { VERIFY(check_consistency()); return false; }
	VERIFY(!flag_read(tp_flags, TypeFlags_Readying));
	flag_set(tp_flags, TypeFlags_Readying);

	if (!tp_base && this != *typeof<OBJECT>()) { tp_base = typeof<OBJECT>(); }
	
	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }
	
	if (!ob_type && tp_base) { ob_type = tp_base->ob_type; }
	
	tp_bases = LIST(tp_base ? ListObject{ tp_base } : ListObject{});
	
	if (!tp_dict) { tp_dict = DICT(DictObject{}); }
	
	VERIFY(mro_internal(nullptr));
	
	if (tp_base) { inherit_special(*tp_base); }

	for (TYPE base : LIST(tp_mro)) { inherit_slots(*base); }

	if (flag_read(tp_flags, TypeFlags_HaveGc | TypeFlags_BaseType) && (!tp_free || tp_free == memfree))
	{
		VERIFY(!"type participates in gc and is a base type but has inappropriate tp_free slot");
	}

	copy_val(*tp_base, &TypeObject::tp_as_number);
	copy_val(*tp_base, &TypeObject::tp_as_sequence);
	copy_val(*tp_base, &TypeObject::tp_as_mapping);

	for (TYPE base : LIST(tp_bases)) { base->add_subclass(this); }

	flag_clear(tp_flags, TypeFlags_Readying);
	flag_set(tp_flags, TypeFlags_Ready);
	return true;
}

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
		for (TYPE base : bases) { result.append(base); }
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
	if (!flag_read(tp_flags, TypeFlags_HaveGc) && flag_read(tp_base->tp_flags, TypeFlags_HaveGc) && (!tp_traverse && !tp_clear))
	{
		flag_set(tp_flags, TypeFlags_HaveGc);
		tp_traverse = tp_base->tp_traverse;
		tp_clear = tp_base->tp_clear;
	}

	if (tp_base != *typeof<OBJECT>() || flag_read(tp_flags, TypeFlags_HeapType) && !tp_new)
	{
		tp_new = tp_base->tp_new;
	}

	copy_val(*tp_base, &TypeObject::tp_size);
	copy_val(*tp_base, &TypeObject::tp_weaklist_offset);
	copy_val(*tp_base, &TypeObject::tp_dict_offset);

	//if (LIST mro{ tp_base->tp_mro }; !mro) { return; }
	//else if (mro->contains(typeof<TYPE>())) { flag_set(tp_flags, TypeFlags_Type_Subclass); }
	//else if (mro->contains(typeof<INT>())) { flag_set(tp_flags, TypeFlags_Int_Subclass); }
	//else if (mro->contains(typeof<FLT>())) { flag_set(tp_flags, TypeFlags_Float_Subclass); }
	//else if (mro->contains(typeof<STR>())) { flag_set(tp_flags, TypeFlags_Str_Subclass); }
	//else if (mro->contains(typeof<LIST>())) { flag_set(tp_flags, TypeFlags_List_Subclass); }
	//else if (mro->contains(typeof<DICT>())) { flag_set(tp_flags, TypeFlags_Dict_Subclass); }
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

	copy_slot(base, basebase, &TypeObject::tp_delete);

	if (!tp_getattr && !tp_getattro) { tp_getattr = base->tp_getattr; tp_getattro = base->tp_getattro; }

	if (!tp_setattr && !tp_setattro) { tp_setattr = base->tp_setattr; tp_setattro = base->tp_setattro; }

	copy_slot(base, basebase, &TypeObject::tp_len);
	copy_slot(base, basebase, &TypeObject::tp_repr);
	copy_slot(base, basebase, &TypeObject::tp_str);

	copy_slot(base, basebase, &TypeObject::tp_vectorcall_offset);
	if (!tp_call && flag_read(base->tp_flags, TypeFlags_HaveVectorCall) && !flag_read(tp_flags, TypeFlags_HeapType))
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
	if (base->tp_descr_get && tp_descr_get == base->tp_descr_get && !flag_read(tp_flags, TypeFlags_HeapType) && flag_read(base->tp_flags, TypeFlags_MethodDescriptor))
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

Error TypeObject::update_slot(OBJECT name) { return Error_None; }

bool TypeObject::check_consistency() const { return true; }

OBJECT TypeObject::lookup(OBJECT name) const
{
	if (isinstance<LIST>(tp_mro))
	{
		for (TYPE base : LIST(tp_mro))
		{
			if (OBJECT * item{ DICT(base->tp_dict).lookup(name) })
			{
				return (*item);
			}
		}
	}
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::api::type_getattro(TYPE type, OBJECT name)
{
	if (!type->tp_dict && !type->ready()) { return nullptr; }

	TYPE metatype{ typeof(type) };

	OBJECT meta_attribute{ metatype->lookup(name) };

	descrgetfunc meta_get{};

	if (TYPE atype; meta_attribute && (atype = typeof(meta_attribute)))
	{
		if ((meta_get = atype->tp_descr_get) && atype->tp_descr_set)
		{
			return meta_get(meta_attribute, type, metatype);
		}
	}

	if (OBJECT attribute{ type->lookup(name) })
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

Error ism::api::type_setattro(TYPE type, OBJECT name, OBJECT value)
{
	Error err{ generic_setattr(type, name, value) };

	if (err == Error_None)
	{
		err = type->update_slot(name);

		VERIFY(type->check_consistency());
	}

	return err;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */