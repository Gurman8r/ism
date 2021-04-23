#include <core/api/types.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreObject_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "object";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattr = (getattrfunc)detail::getattr_string;
	t.tp_setattr = (setattrfunc)detail::setattr_string;
	t.tp_getattro = (getattrofunc)detail::getattr_object;
	t.tp_setattro = (setattrofunc)detail::setattr_object;

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreObject *)ptr); };
};

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
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_name = ***STR(value)), Error_None; },
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
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return (TYPE(self)->tp_doc = ***STR(value)), Error_None; },
	},
	{ /* sentinal */ },
};

static MethodDef type_methods[] =
{
	{ "mro", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return TYPE(self)->tp_mro;
	} },
	{ "__subclasses__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return TYPE(self)->tp_subclasses;
	} },
	{ "__instancecheck__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return nullptr;
	} },
	{ "__subclasscheck__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return nullptr;
	} },
	{ "__dir__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return nullptr;
	} },
	{ "__sizeof__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return nullptr;
	} },
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

CoreType ism::_CoreType_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "type";
	t.tp_basicsize = sizeof(CoreType);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_Type_Subclass;

	t.tp_dict_offset = offsetof(CoreType, tp_dict);
	t.tp_weaklist_offset = offsetof(CoreType, tp_weaklist);
	t.tp_vectorcall_offset = offsetof(CoreType, tp_vectorcall);

	t.tp_hash = (hashfunc)[](OBJECT o) { return hash(TYPE(o)->tp_name); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(TYPE(o)->tp_name); };

	t.tp_getattr = (getattrfunc)detail::getattr_string;
	t.tp_setattr = (setattrfunc)detail::setattr_string;
	t.tp_getattro = (getattrofunc)detail::getattr_object;
	t.tp_setattro = (setattrofunc)detail::setattr_object;

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreType *)ptr); };

	t.tp_getsets = type_getsets;
	t.tp_methods = type_methods;

	t.tp_as_number = &type_as_number;
	t.tp_as_sequence = &type_as_sequence;
	t.tp_as_mapping = &type_as_mapping;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<TYPE>(v)) return util::compare(TYPE(o)->tp_name, TYPE(v)->tp_name);
		return util::compare(o.ptr(), v.ptr());
	};
};

bool CoreType::ready()
{
	if (has_feature(TypeFlags_Ready)) { return true; }

	if (!tp_base && this != &_CoreObject_Type) { tp_base = &_CoreObject_Type; }

	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }

	if (!ob_type && tp_base) { ob_type = tp_base->ob_type; }

	tp_bases = LIST(tp_base ? CoreList{ tp_base } : CoreList{});

	if (!tp_dict) { tp_dict = DICT(CoreDict{}); }

	if (tp_methods) { add_methods(tp_methods); }

	if (tp_getsets) { add_getsets(tp_getsets); }

	VERIFY(mro_internal(nullptr));

	if (tp_base) { inherit_special(*tp_base); }

	for (TYPE const & base : ***LIST(tp_mro))
	{
		inherit_slots(*base);
	}

	if (!has_feature(TypeFlags_HeapType))
	{
		for (TYPE const & b : ***LIST(tp_mro))
		{
			VERIFY("static type has dynamically allocated base type" && !b->has_feature(TypeFlags_HeapType));
		}
	}

	if (has_feature(TypeFlags_HaveGc | TypeFlags_BaseType) && (!tp_free || tp_free == memfree))
	{
		VERIFY(!"type participates in gc and is a base type but has inappropriate tp_free slot");
	}

	copy_slot(*tp_base, &CoreType::tp_as_number);
	copy_slot(*tp_base, &CoreType::tp_as_sequence);
	copy_slot(*tp_base, &CoreType::tp_as_mapping);

	for (TYPE base : ***LIST(tp_bases))
	{
		base->add_subclass(handle());
	}

	enable_feature(TypeFlags_Ready);
	return true;
}

void CoreType::add_methods(MethodDef * methods)
{
	for (MethodDef * def{ methods }; def && *def; ++def)
	{
	}
}

void CoreType::add_getsets(GetSetDef * getsets)
{
	for (GetSetDef * def{ getsets }; def && *def; ++def)
	{
	}
}

bool CoreType::add_subclass(TYPE const & type)
{
	if (!tp_subclasses) { tp_subclasses = DICT(CoreDict{}); }

	return (***DICT(tp_subclasses)).insert_or_assign(type, type).second;
}

bool CoreType::mro_internal(OBJECT * old_mro)
{
	tp_mro = LIST(CoreList{});
	return true;
}

void CoreType::inherit_special(CoreType const * base)
{
	if (!has_feature(TypeFlags_HaveGc) && tp_base->has_feature(TypeFlags_HaveGc) && (!tp_traverse && !tp_clear))
	{
		enable_feature(TypeFlags_HaveGc);
		tp_traverse = tp_base->tp_traverse;
		tp_clear = tp_base->tp_clear;
	}

	if (tp_base != &_CoreObject_Type || has_feature(TypeFlags_HeapType) && !tp_new)
	{
		tp_new = tp_base->tp_new;
	}

	copy_slot(*tp_base, &CoreType::tp_basicsize);
	copy_slot(*tp_base, &CoreType::tp_itemsize);
	copy_slot(*tp_base, &CoreType::tp_weaklist_offset);
	copy_slot(*tp_base, &CoreType::tp_dict_offset);

	if (LIST mro{ tp_base->tp_mro }; !mro) { return; }
	else if (mro->contains(CoreType_Type)) { enable_feature(TypeFlags_Type_Subclass); }
	else if (mro->contains(CoreInt_Type)) { enable_feature(TypeFlags_Int_Subclass); }
	else if (mro->contains(CoreFloat_Type)) { enable_feature(TypeFlags_Float_Subclass); }
	else if (mro->contains(CoreString_Type)) { enable_feature(TypeFlags_Str_Subclass); }
	else if (mro->contains(CoreList_Type)) { enable_feature(TypeFlags_List_Subclass); }
	else if (mro->contains(CoreDict_Type)) { enable_feature(TypeFlags_Dict_Subclass); }
}

void CoreType::inherit_slots(CoreType const * base)
{
	CoreType const * basebase{};

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

	if (!tp_getattr && !tp_getattro) { tp_getattr = base->tp_getattr; tp_getattro = base->tp_getattro; }

	if (!tp_setattr && !tp_setattro) { tp_setattr = base->tp_setattr; tp_setattro = base->tp_setattro; }

	copy_slot(base, basebase, &CoreType::tp_len);
	copy_slot(base, basebase, &CoreType::tp_repr);
	copy_slot(base, basebase, &CoreType::tp_str);
	
	copy_slot(base, basebase, &CoreType::tp_vectorcall_offset);
	if (!tp_call && base->has_feature(TypeFlags_HaveVectorCall) && !has_feature(TypeFlags_HeapType))
	{
		enable_feature(TypeFlags_HaveVectorCall);
	}
	copy_slot(base, basebase, &CoreType::tp_call);
	
	if (!tp_compare && !tp_hash && !tp_dict->contains("__eq__") && !tp_dict->contains("__hash__"))
	{
		tp_compare = base->tp_compare;
		tp_hash = base->tp_hash;
	}

	copy_slot(base, basebase, &CoreType::tp_descrget);
	if (base->tp_descrget && tp_descrget == base->tp_descrget && !has_feature(TypeFlags_HeapType) && base->has_feature(TypeFlags_MethodDescriptor))
	{
		enable_feature(TypeFlags_MethodDescriptor);
	}
	copy_slot(base, basebase, &CoreType::tp_descrset);

	copy_slot(base, basebase, &CoreType::tp_dict_offset);
	copy_slot(base, basebase, &CoreType::tp_init);
	copy_slot(base, basebase, &CoreType::tp_alloc);
	copy_slot(base, basebase, &CoreType::tp_is_gc);
	copy_slot(base, basebase, &CoreType::tp_finalize);
	copy_slot(base, basebase, &CoreType::tp_free);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreNone_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "none";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_base = CoreObject_Type;
	t.tp_flags = TypeFlags_Default;
	t.tp_doc = "";

	t.tp_delete = (destructor)[](CoreObject * ptr) {};
};

CoreObject ism::_Core_None = COMPOSE(CoreObject, v) { v.ob_type = CoreNone_Type; };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static NumberMethods bool_as_number = COMPOSE(NumberMethods, m)
{
};

CoreType ism::_CoreBool_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "bool";
	t.tp_basicsize = sizeof(CoreInt);
	t.tp_base = CoreInt_Type;
	t.tp_as_number = &bool_as_number;
	t.tp_delete = (destructor)[](CoreObject * ptr) {};
};

CoreInt ism::_Core_True = COMPOSE(CoreInt, v) { v.ob_type = CoreBool_Type; v = 1; };

CoreInt ism::_Core_False = COMPOSE(CoreInt, v) { v.ob_type = CoreBool_Type; v = 0; };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreInt_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "int";
	t.tp_basicsize = sizeof(CoreInt);
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<int64_t>()(***INT(o)); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(to_string(***INT(o))); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(to_string(***INT(o))); };

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreInt *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<INT>(v)) return util::compare(***INT(o), ***INT(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreFloat_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "float";
	t.tp_basicsize = sizeof(CoreFloat);
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<double_t>()(***FLT(o)); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(to_string(***FLT(o))); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(to_string(***FLT(o))); };

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreFloat *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<FLT>(v)) return util::compare(***FLT(o), ***FLT(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreString_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "str";
	t.tp_basicsize = sizeof(CoreString);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return hash(***STR(o)); };
	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)STR(o)->size(); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(o); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(o); };

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreString *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<STR>(v)) return util::compare(***STR(o), ***STR(v));
		else if (auto s{ STR({ v }) }) return util::compare(***STR(o), ***s);
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static MethodDef list_methods[] =
{
	{ "__contains__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(LIST(self)->contains(value));
	} },
	{ /* sentinal */ },
};

CoreType ism::_CoreList_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "list";
	t.tp_basicsize = sizeof(CoreList);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)LIST(o)->size(); };

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreList *)ptr); };

	t.tp_methods = list_methods;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<LIST>(v)) return util::compare(***LIST(o), ***LIST(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static MethodDef dict_methods[] =
{
	{ "__contains__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(DICT(self)->contains(value));
	} },
	{ /* sentinal */ },
};

CoreType ism::_CoreDict_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(CoreDict);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o)->size(); };

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreDict *)ptr); };

	t.tp_methods = dict_methods;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef capsule_getsets[] =
{
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CAPSULE(self)).name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CAPSULE(self)).name = ***STR(value)), Error_None; },
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CAPSULE(self)).doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CAPSULE(self)).doc = ***STR(value)), Error_None; },
	},
	{ /* sentinal */ },
};

CoreType ism::_CoreCapsule_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "capsule";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreCapsule *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef function_getsets[] =
{
	{ "__dict__",
		(getter)[](OBJECT self, void *) -> OBJECT { return FUNCTION(self)->m_dict; },
	},
	{ /* sentinal */ },
};

CoreType ism::_CoreFunction_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "function";
	t.tp_basicsize = sizeof(CoreFunction);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;

	t.tp_dict_offset = offsetof(CoreFunction, m_dict);
	t.tp_vectorcall_offset = offsetof(CoreFunction, m_vectorcall);

	t.tp_getattr = (getattrfunc)detail::getattr_string;
	t.tp_setattr = (setattrfunc)detail::setattr_string;
	t.tp_getattro = (getattrofunc)detail::getattr_object;
	t.tp_setattro = (setattrofunc)detail::setattr_object;

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreFunction *)ptr); };

	t.tp_getsets = function_getsets;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef property_getsets[] =
{
	{ /* sentinal */ },
};

CoreType ism::_CoreProperty_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "property";
	t.tp_basicsize = sizeof(CoreProperty);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreProperty *)ptr); };

	t.tp_getsets = property_getsets;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */