#include <core/api/object/type_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef type_getsets[] =
{
	{ "__basicsize__",
		(getter)[](OBJECT self, void *) -> OBJECT { return INT(TYPE(self)->tp_basicsize); },
	},
	{ "__itemsize__",
		(getter)[](OBJECT self, void *) -> OBJECT { return INT(TYPE(self)->tp_itemsize); },
	},
	{ "__flags__",
		(getter)[](OBJECT self, void *) -> OBJECT { return INT(TYPE(self)->tp_flags); },
	},
	{ "__base__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_base; },
	},
	{ "__mro__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_mro; },
	},
	{ "__dict__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_dict; }
	},
	{ "__weaklist__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_weaklist; },
	},
	{ "__text_signature__",
		(getter)[](OBJECT self, void *) -> OBJECT { return nullptr; },
	},
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR(TYPE(self)->tp_name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_name = (String)STR(value)), Error_None; },
	},
	{ "__qualname__",
		(getter)[](OBJECT self, void *) -> OBJECT { return nullptr; },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return Error_None; },
	},
	{ "__bases__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_bases; },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_bases = value), Error_None; },
	},
	{ "__module__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_dict["__module__"]; },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_dict["__module__"] = value), Error_None; },
	},
	{ "__abstractmethods__",
		(getter)[](OBJECT self, void *) -> OBJECT { return TYPE(self)->tp_dict["__abstractmethods__"]; },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_dict["__abstractmethods__"] = value), Error_None; },
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR(TYPE(self)->tp_doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_doc = (String)STR(value)), Error_None; },
	},
	{ /* sentinal */ },
};

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

STATIC_MEMBER(TypeObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "type";
	t.tp_basicsize = sizeof(TypeObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_Type_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_dict_offset = offsetof(TypeObject, tp_dict);
	t.tp_weaklist_offset = offsetof(TypeObject, tp_weaklist);
	t.tp_vectorcall_offset = offsetof(TypeObject, tp_vectorcall);

	t.tp_hash = (hashfunc)[](OBJECT o) { return hash(TYPE(o)->tp_name); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };

	t.tp_getattr = (getattrfunc)impl_getattr_string;
	t.tp_setattr = (setattrfunc)impl_setattr_string;
	t.tp_getattro = (getattrofunc)impl_getattr_object;
	t.tp_setattro = (setattrofunc)impl_setattr_object;

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

	t.tp_getsets = type_getsets;

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

void TypeObject::_bind_class(TypeObject & t)
{
	t.attr("test") = CPP_FUNCTION([]()
	{
		printf("%s\n", PRETTY_FUNCTION);
	});

	t.attr("mro") = CPP_FUNCTION([](TYPE self)
	{
	});

	t.attr("__contains__") = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
		return DICT(self->tp_dict)->contains(value);
	});

	t.attr("__instancecheck__") = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
	});

	t.attr("__subclasscheck__") = CPP_FUNCTION([](TYPE self, OBJECT value)
	{
	});

	t.attr("__dir__") = CPP_FUNCTION([](TYPE self)
	{
	});

	t.attr("__sizeof__") = CPP_FUNCTION([](TYPE self)
	{
	});

	t.attr("__subclasses__") = CPP_FUNCTION([](TYPE self)
	{
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::ready()
{
	if (has_feature(TypeFlags_Ready)) { return true; }

	if (!tp_base && this != *typeof<OBJECT>()) { tp_base = typeof<OBJECT>(); }

	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }

	if (!ob_type && tp_base) { ob_type = tp_base->ob_type; }

	tp_bases = LIST(tp_base ? ListObject{ tp_base } : ListObject{});

	if (!tp_dict) { tp_dict = DICT(DictObject{}); }

	VERIFY(mro_internal(nullptr));

	if (tp_base) { inherit_special(*tp_base); }

	if (LIST mro{ LIST(tp_mro) }; mro && !mro.empty())
	{
		for (TYPE b : ***mro)
		{
			inherit_slots(*b);
		}
		
		if (!has_feature(TypeFlags_HeapType))
		{
			for (TYPE b : ***mro)
			{
				VERIFY("static type has dynamically allocated base type" && !b->has_feature(TypeFlags_HeapType));
			}
		}
	}

	if (has_feature(TypeFlags_HaveGc | TypeFlags_BaseType) && (!tp_free || tp_free == memfree))
	{
		VERIFY(!"type participates in gc and is a base type but has inappropriate tp_free slot");
	}

	copy_val(*tp_base, &TypeObject::tp_as_number);
	copy_val(*tp_base, &TypeObject::tp_as_sequence);
	copy_val(*tp_base, &TypeObject::tp_as_mapping);

	for (TYPE b : ***LIST(tp_bases))
	{
		b->add_subclass(this);
	}

	return enable_feature(TypeFlags_Ready), true;
}

bool TypeObject::add_subclass(TypeObject const * type)
{
	if (!tp_subclasses) { tp_subclasses = DICT(DictObject{}); }

	//return (***DICT(tp_subclasses)).insert_or_assign(type, type).second;

	return false;
}

bool TypeObject::mro_internal(OBJECT * old_mro)
{
	if (tp_base) { tp_mro = LIST(ListObject{ tp_base }); }
	
	return true;
}

void TypeObject::inherit_special(TypeObject const * base)
{
	if (!has_feature(TypeFlags_HaveGc) && tp_base->has_feature(TypeFlags_HaveGc) && (!tp_traverse && !tp_clear))
	{
		enable_feature(TypeFlags_HaveGc);
		tp_traverse = tp_base->tp_traverse;
		tp_clear = tp_base->tp_clear;
	}

	if (tp_base != *typeof<OBJECT>() || has_feature(TypeFlags_HeapType) && !tp_new)
	{
		tp_new = tp_base->tp_new;
	}

	copy_val(*tp_base, &TypeObject::tp_basicsize);
	copy_val(*tp_base, &TypeObject::tp_itemsize);
	copy_val(*tp_base, &TypeObject::tp_weaklist_offset);
	copy_val(*tp_base, &TypeObject::tp_dict_offset);

	if (LIST mro{ tp_base->tp_mro }; !mro) { return; }
	else if (mro->contains(typeof<TYPE>())) { enable_feature(TypeFlags_Type_Subclass); }
	else if (mro->contains(typeof<INT>())) { enable_feature(TypeFlags_Int_Subclass); }
	else if (mro->contains(typeof<FLT>())) { enable_feature(TypeFlags_Float_Subclass); }
	else if (mro->contains(typeof<STR>())) { enable_feature(TypeFlags_Str_Subclass); }
	else if (mro->contains(typeof<LIST>())) { enable_feature(TypeFlags_List_Subclass); }
	else if (mro->contains(typeof<DICT>())) { enable_feature(TypeFlags_Dict_Subclass); }
}

void TypeObject::inherit_slots(TypeObject const * base)
{
	TypeObject const * basebase{};

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
	if (!tp_call && base->has_feature(TypeFlags_HaveVectorCall) && !has_feature(TypeFlags_HeapType))
	{
		enable_feature(TypeFlags_HaveVectorCall);
	}
	copy_slot(base, basebase, &TypeObject::tp_call);

	if (!tp_compare && !tp_hash && !tp_dict->contains("__eq__") && !tp_dict->contains("__hash__"))
	{
		tp_compare = base->tp_compare;
		tp_hash = base->tp_hash;
	}

	copy_slot(base, basebase, &TypeObject::tp_descr_get);
	if (base->tp_descr_get && tp_descr_get == base->tp_descr_get && !has_feature(TypeFlags_HeapType) && base->has_feature(TypeFlags_MethodDescriptor))
	{
		enable_feature(TypeFlags_MethodDescriptor);
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