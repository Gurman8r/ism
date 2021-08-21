#include <core/api/object/type_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TypeObject::TypeObject(TYPE type) noexcept : Object{ type } {}

TypeObject::TypeObject() noexcept : TypeObject{ get_class_static() } {}

TYPE TypeObject::get_class_static() noexcept { return &_class_type_static; }

TYPE TypeObject::_get_typev() const noexcept { return get_class_static(); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static MemberDef type_members[] = { {/* sentinal */}, };

static MethodDef type_methods[] = { {/* sentinal */}, };

static GetSetDef type_getsets[] = { {/* sentinal */}, };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static NumberMethods type_as_number = COMPOSE(NumberMethods, m) {};

static MappingMethods type_as_mapping = COMPOSE(MappingMethods, m) {};

static SequenceMethods type_as_sequence = COMPOSE(SequenceMethods, m) {};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(TypeObject, t)
{
	t.tp_name = "type";
	
	t.tp_size = sizeof(TypeObject);
	
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_Type_Subclass;
	
	t.tp_dictoffset = offsetof(TypeObject, tp_dict);
	
	t.tp_vectorcalloffset = offsetof(TypeObject, tp_vectorcall);

	t.tp_as_number = &type_as_number;
	
	t.tp_as_mapping = &type_as_mapping;
	
	t.tp_as_sequence = &type_as_sequence;

	t.tp_hash = (hashfunc)[](OBJ self) { return ism::hash(TYPE(self)->tp_name); };
	
	t.tp_repr = (reprfunc)[](OBJ self) { return STR(TYPE(self)->tp_name); };
	
	t.tp_str = (reprfunc)[](OBJ self) { return STR(TYPE(self)->tp_name); };
	
	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };

	t.tp_getattro = (getattrofunc)type_getattro;
	
	t.tp_setattro = (setattrofunc)type_setattro;

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(TypeObject); };
	
	t.tp_del = (delfunc)[](Object * ptr) { memdelete((TypeObject *)ptr); };

	t.tp_call = (binaryfunc)[](OBJ self, OBJ args) -> OBJ
	{
		VERIFY(TYPE::check_(self));
		
		newfunc create{ TYPE(self)->tp_new };
		
		if (!create) { return nullptr; }
		
		return create(self, args);
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void TypeObject::_bind_methods()
{
	CLASS_<TYPE>()

		.def("__contains__", [](TYPE self, OBJ value) { return DICT(self->tp_dict).contains(value); })

		.def("__instancecheck__", [](OBJ inst, OBJ cls) { return isinstance(inst, cls); })

		.def("__subclasscheck__", &TYPE::is_subtype)

		.def_property_readonly("__dictoffset__", [](TYPE self) { return self->tp_dictoffset; })

		.def_property_readonly("__size__", [](TYPE self) { return self->tp_size; })
	
		.def_property_readonly("__flags__", [](TYPE self) { return self->tp_flags; })
	
		.def_property_readonly("__base__", [](TYPE self) { return TYPE(self->tp_base); })
	
		.def_property_readonly("__mro__", [](TYPE self) { return self->tp_mro; })
	
		.def_property_readonly("__dict__", [](TYPE self) { return self->tp_dict; })
	
		.def_property_readonly("__text_signature__", [](TYPE self) { return STR(/* TODO */); })
	
		.def_property_readonly("__qualname__", [](TYPE self) { return STR(/* TODO */); })
	
		.def_property("__name__", [](TYPE self) { return self->tp_name; }, [](TYPE self, STR value) { self->tp_name = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::ready()
{
	if (tp_flags & TypeFlags_Ready) { VERIFY(check_consistency()); return true; }
	
	VERIFY(!(tp_flags & TypeFlags_Readying));

	tp_flags |= TypeFlags_Readying;

	if (!tp_base && this != *typeof<OBJ>()) { tp_base = typeof<OBJ>(); }
	
	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }
	
	if (!ob_type && tp_base) { ob_type = tp_base->ob_type; }
	
	tp_bases = tp_base ? LIST::new_(tp_base) : LIST::new_();

	if (!tp_dict) { tp_dict = DICT::new_(); }
	
	VERIFY(mro_internal(nullptr));
	
	if (tp_base) { inherit_special(*tp_base); }

	for (TYPE const & base : LIST(tp_mro)) { inherit_slots(*base); }

	copy_val(*tp_base, &TypeObject::tp_as_number);
	
	copy_val(*tp_base, &TypeObject::tp_as_sequence);
	
	copy_val(*tp_base, &TypeObject::tp_as_mapping);

	for (TYPE const & base : LIST(tp_bases)) { base->add_subclass(this); }

	tp_flags = (tp_flags & ~TypeFlags_Readying) | TypeFlags_Ready;

	return true;
}

OBJ TypeObject::lookup(OBJ name) const
{
	if (LIST mro; !name || !(mro = tp_mro)) { return nullptr; }
	else
	{
		for (TYPE const & base : mro)
		{
			if (OBJ item{ DICT(base->tp_dict).lookup(name) })
			{
				return item;
			}
		}
		return nullptr;
	}
}

bool TypeObject::is_subtype(TYPE value) const
{
	if (LIST mro{ tp_mro })
	{
		for (TYPE const & base : mro)
		{
			if (value.is(base)) { return true; }
		}

		return false;
	}
	else
	{
		TypeObject const * base{ this };

		do {

			if (value.is(base)) { return true; }

			base = *base->tp_base;

		} while (base);

		return value.is(typeof<OBJ>());
	}
}

bool TypeObject::check_consistency() const
{
	// don't check static types before ready()
	if (!(tp_flags & TypeFlags_Ready)) { return true; }

	if (!TYPE::check_(ptr())) {
		FATAL("");
	}

	if (tp_flags & TypeFlags_Readying) {
		FATAL("");
	}

	if (!tp_dict) {
		FATAL("");
	}

	return true;
}

void TypeObject::modified()
{
}

Error TypeObject::update_slot(STR name)
{
	if (!name) { return Error_Unknown; }
	switch (hash((String)name))
	{
	default: return Error_None;

	case hash("__new__"): { tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ {
		if (ISM_IDENTIFIER(__new__); OBJ f{ type.lookup(&ID___new__) }) { return call_object(f, args); }
		return nullptr;
	}; } break;

	case hash("__del__"): {} break;

	case hash("__getattr__"): {} break;
	case hash("__setattr__"): {} break;
	case hash("__delattr__"): {} break;

	case hash("__get__"): { tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ {
		if (ISM_IDENTIFIER(__get__); OBJ f{ typeof(self).lookup(&ID___get__) }) { return call_object(f, LIST({ { self, obj, cls } })); }
		return nullptr;
	}; } break;

	case hash("__set__"): { tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ cls) -> Error {
		if (ISM_IDENTIFIER(__set__); OBJ f{ typeof(self).lookup(&ID___set__) }) { return call_object(f, LIST({ { self, obj, cls } })), Error_None; }
		return Error_Unknown;
	}; } break;

	case hash("__call__"): { tp_call = (binaryfunc)[](OBJ self, OBJ args) -> OBJ {
		if (ISM_IDENTIFIER(__call__); OBJ f{ typeof(self).lookup(&ID___call__) }) { return call_object(f, args); }
		return nullptr;
	}; } break;

	case hash("__hash__"): { tp_hash = (hashfunc)[](OBJ self) -> hash_t {
		if (ISM_IDENTIFIER(__hash__); OBJ f{ typeof(self).lookup(&ID___hash__) }) { return call_object(f, self).cast<hash_t>(); }
		return 0;
	}; } break;

	case hash("__len__"): { tp_len = (lenfunc)[](OBJ self) -> ssize_t {
		if (ISM_IDENTIFIER(__len__); OBJ f{ typeof(self).lookup(&ID___len__) }) { return call_object(f, self).cast<ssize_t>(); }
		return -1;
	}; } break;

	case hash("__repr__"): { tp_repr = (reprfunc)[](OBJ self) -> STR {
		if (ISM_IDENTIFIER(__repr__); OBJ f{ typeof(self).lookup(&ID___repr__) }) { return call_object(f, self); }
		return nullptr;
	}; } break;

	case hash("__str__"): { tp_str = (reprfunc)[](OBJ self) -> STR {
		if (ISM_IDENTIFIER(__str__); OBJ f{ typeof(self).lookup(&ID___str__) }) { return call_object(f, self); }
		return nullptr;
	}; } break;

	}
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::add_subclass(TypeObject * type)
{
	if (!tp_subclasses) { tp_subclasses = DICT::new_(); }

	(DICT(tp_subclasses))[type] = OBJ(type);

	return true;
}

bool TypeObject::mro_internal(OBJ * in_old_mro)
{
	OBJ old_mro{ tp_mro };

	OBJ new_mro{ std::invoke([&, &bases = LIST(tp_bases)]()->OBJ
	{
		// mro_implementation
		LIST result{ LIST::new_() };
		result.reserve(bases.size() + 1);
		result.append(ptr());
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
	if (typeof<OBJ>() != base || (tp_flags & TypeFlags_HeapType) && !tp_new)
	{
		tp_new = base->tp_new;
	}

	copy_val(base, &TypeObject::tp_size);

	copy_val(base, &TypeObject::tp_dictoffset);

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

	copy_slot(base, basebase, &TypeObject::tp_del);

	if (!tp_getattr && !tp_getattro) { tp_getattr = base->tp_getattr; tp_getattro = base->tp_getattro; }

	if (!tp_setattr && !tp_setattro) { tp_setattr = base->tp_setattr; tp_setattro = base->tp_setattro; }

	copy_slot(base, basebase, &TypeObject::tp_len);
	
	copy_slot(base, basebase, &TypeObject::tp_repr);
	
	copy_slot(base, basebase, &TypeObject::tp_str);

	copy_slot(base, basebase, &TypeObject::tp_vectorcalloffset);
	
	if (!tp_call && (base->tp_flags & TypeFlags_HaveVectorCall) && !(tp_flags & TypeFlags_HeapType))
	{
		flag_set(tp_flags, TypeFlags_HaveVectorCall);
	}

	copy_slot(base, basebase, &TypeObject::tp_call);

	if (!tp_compare && !tp_hash)
	{
		ISM_IDENTIFIER(__eq__);
		ISM_IDENTIFIER(__hash__);
		if (DICT dict{ tp_dict }; dict && !dict.contains(&ID___eq__) && !dict.contains(&ID___hash__))
		{
			tp_compare = base->tp_compare;

			tp_hash = base->tp_hash;
		}
	}

	copy_slot(base, basebase, &TypeObject::tp_descr_get);

	if (base->tp_descr_get && tp_descr_get == base->tp_descr_get && !(tp_flags & TypeFlags_HeapType) && (base->tp_flags & TypeFlags_MethodDescriptor))
	{
		flag_set(tp_flags, TypeFlags_MethodDescriptor);
	}

	copy_slot(base, basebase, &TypeObject::tp_descr_set);

	copy_slot(base, basebase, &TypeObject::tp_dictoffset);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::type_getattro(TYPE type, OBJ name)
{
	return generic_getattr(type, name);

	// FIXME

	if (!type->tp_dict && !type.ready()) { return nullptr; }

	TYPE metatype{ typeof(type) };

	OBJ meta_attribute{ metatype.lookup(name) };

	descrgetfunc meta_get{};

	if (meta_attribute)
	{
		TYPE meta_attr_type{ typeof(meta_attribute) };

		meta_get = meta_attr_type->tp_descr_get;

		if (meta_get && meta_attr_type->tp_descr_set)
		{
			return meta_get(meta_attribute, type, metatype);
		}
	}

	if (OBJ attribute{ type.lookup(name) })
	{
		TYPE attr_type{ typeof(attribute) };

		if (descrgetfunc local_get{ attr_type->tp_descr_get })
		{
			return local_get(attribute, nullptr, type);
		}
	}

	if (meta_get) { return meta_get(meta_attribute, type, metatype); }

	if (meta_attribute) { return meta_attribute; }

	return nullptr;
}

Error ism::type_setattro(TYPE type, OBJ name, OBJ value)
{
	VERIFY(STR::check_(name));

	Error err{ generic_setattr(type, name, value) };

	if (err == Error_None)
	{
		type->modified();

		if (is_dunder_name(name))
		{
			err = type->update_slot(name);
		}

		VERIFY(type->check_consistency());
	}

	return err;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */