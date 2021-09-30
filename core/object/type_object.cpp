#include <core/object/type_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void TypeObject::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		api::get_internals().add_class(&g_class_type);

		VALIDATE(g_class_type.tp_bind)(&g_class_type);
	};
}

void TypeObject::_initialize_classv() { TypeObject::initialize_class(); }

TYPE TypeObject::_get_typev() const noexcept { return get_type_static(); }

TYPE TypeObject::get_type_static() noexcept { return &g_class_type; }

TypeObject::TypeObject() noexcept : Object{} {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(TypeObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_Type_Subclass)
{
	t.tp_dictoffset = offsetof(TypeObject, tp_dict);

	t.tp_vectorcalloffset = offsetof(TypeObject, tp_vectorcall);

	t.tp_getattro = (getattrofunc)api::type_getattro;

	t.tp_setattro = (setattrofunc)api::type_setattro;

	t.tp_hash = (hashfunc)[](OBJ self) { return ism::hash(TYPE(self)->tp_name); };

	t.tp_repr = (reprfunc)[](OBJ self) { return STR(TYPE(self)->tp_name); };

	t.tp_str = (reprfunc)[](OBJ self) { return STR(TYPE(self)->tp_name); };

	t.tp_call = (binaryfunc)[](OBJ self, OBJ args) -> OBJ
	{
		VERIFY(TYPE::check_(self));

		if (newfunc fn{ TYPE(self)->tp_new }; fn) { return fn(self, args); }

		return nullptr;
	};

	t.tp_bind = CLASS_BINDFUNC(TypeObject, t)
	{
		return t

			.def_static("__instancecheck__", [](OBJ const & obj, OBJ const & type) { return api::isinstance(obj, type); })

			.def("__contains__", [](TypeObject const & self, OBJ const & value) { return DICT(self.tp_dict).contains(value); })

			.def("__subclasscheck__", &TypeObject::is_subtype)

			.def_readonly("__base__", &TypeObject::tp_base)

			.def_readonly("__dict__", &TypeObject::tp_dict)

			.def_readonly("__dictoffset__", &TypeObject::tp_dictoffset)

			.def_readonly("__flags__", &TypeObject::tp_flags)

			.def_readonly("__mro__", &TypeObject::tp_mro)

			.def_readwrite("__name__", &TypeObject::tp_name)

			.def_readonly("__size__", &TypeObject::tp_size)

			.def_readonly("__vectorcalloffset__", &TypeObject::tp_vectorcalloffset)

			.def_property_readonly("__text_signature__", [](TypeObject const & self) { return STR(/* TODO */); })

			.def_property_readonly("__qualname__", [](TypeObject const & self) { return STR(/* TODO */); })

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool TypeObject::ready()
{
	if (tp_flags & TypeFlags_Ready) { VERIFY(check_consistency()); return true; }
	
	VERIFY(!(tp_flags & TypeFlags_Readying));

	tp_flags |= TypeFlags_Readying;

	if (!tp_base && this != *api::typeof<OBJ>()) { tp_base = api::typeof<OBJ>(); }
	
	if (tp_base && !tp_base->tp_dict) { VERIFY(tp_base->ready()); }
	
	if (!get_type() && tp_base) { set_type(tp_base->get_type()); }
	
	tp_bases = tp_base ? LIST::new_(tp_base) : LIST::new_();

	if (!tp_dict) { tp_dict = DICT::new_(); }

	VERIFY(mro_internal(nullptr));
	
	if (tp_base) { inherit_special(*tp_base); }

	for (TYPE const & base : LIST(tp_mro)) { inherit_slots(*base); }

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

			base = base->tp_base.ptr();

		} while (base);

		return value.is(api::typeof<OBJ>());
	}
}

bool TypeObject::check_consistency() const
{
	// don't check static types before ready()
	if (!(tp_flags & TypeFlags_Ready)) { return true; }

	VERIFY(TYPE::check_(ptr()));

	VERIFY(!(tp_flags & TypeFlags_Readying));

	VERIFY(tp_dict);

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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	case hash("__getattr__"):
	case hash("__setattr__"):
	case hash("__delattr__"):
	default: return Error_None;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	case hash("__new__"): { tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ {
		if (STR_IDENTIFIER(__new__); OBJ f{ type.lookup(&ID___new__) }) { return api::call_object(f, args); }
		return nullptr;
	}; } break;

	case hash("__del__"): { tp_del = (delfunc)[](Object * obj) -> void {
		if (STR_IDENTIFIER(__del__); OBJ f{ api::typeof(obj).lookup(&ID___del__) })
		{
		}
	}; } break;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	case hash("__call__"): { tp_call = (binaryfunc)[](OBJ self, OBJ args) -> OBJ {
		if (STR_IDENTIFIER(__call__); OBJ f{ api::typeof(self).lookup(&ID___call__) }) { return api::call_object(f, args); }
		return nullptr;
	}; } break;

	case hash("__hash__"): { tp_hash = (hashfunc)[](OBJ self) -> hash_t {
		if (STR_IDENTIFIER(__hash__); OBJ f{ api::typeof(self).lookup(&ID___hash__) }) { return api::call_object(f, self).cast<hash_t>(); }
		return 0;
	}; } break;

	case hash("__len__"): { tp_len = (lenfunc)[](OBJ self) -> ssize_t {
		if (STR_IDENTIFIER(__len__); OBJ f{ api::typeof(self).lookup(&ID___len__) }) { return api::call_object(f, self).cast<ssize_t>(); }
		return -1;
	}; } break;

	case hash("__repr__"): { tp_repr = (reprfunc)[](OBJ self) -> STR {
		if (STR_IDENTIFIER(__repr__); OBJ f{ api::typeof(self).lookup(&ID___repr__) }) { return api::call_object(f, self); }
		return nullptr;
	}; } break;

	case hash("__str__"): { tp_str = (reprfunc)[](OBJ self) -> STR {
		if (STR_IDENTIFIER(__str__); OBJ f{ api::typeof(self).lookup(&ID___str__) }) { return api::call_object(f, self); }
		return nullptr;
	}; } break;

	case hash("__get__"): { tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ {
		if (STR_IDENTIFIER(__get__); OBJ f{ api::typeof(self).lookup(&ID___get__) }) { /* TODO */ }
		return nullptr;
	}; } break;

	case hash("__set__"): { tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ type) -> Error {
		if (STR_IDENTIFIER(__set__); OBJ f{ api::typeof(self).lookup(&ID___set__) }) { /* TODO */ }
		return Error_Unknown;
	}; } break;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	case hash("__eq__"): {} break;
	case hash("__ne__"): {} break;
	case hash("__lt__"): {} break;
	case hash("__le__"): {} break;
	case hash("__gt__"): {} break;
	case hash("__ge__"): {} break;

	case hash("__add__"): {} break;
	case hash("__sub__"): {} break;
	case hash("__div__"): {} break;
	case hash("__mul__"): {} break;
	case hash("__mod__"): {} break;
	case hash("__pow__"): {} break;
	case hash("__pos__"): {} break;
	case hash("__neg__"): {} break;

	case hash("__invert__"): {} break;
	case hash("__lshift__"): {} break;
	case hash("__rshift__"): {} break;
	case hash("__and__"): {} break;
	case hash("__or__"): {} break;
	case hash("__xor__"): {} break;

	case hash("__int__"): {} break;
	case hash("__float__"): {} break;
	case hash("__bool__"): {} break;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

	OBJ new_mro{ std::invoke([&]()->OBJ
	{
		// mro_implementation
		LIST bases{ tp_bases };
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
	if (api::typeof<OBJ>() != base || (tp_flags & TypeFlags_HeapType) && !tp_new)
	{
		tp_new = base->tp_new;
	}

	copy_val(base, &TypeObject::tp_size);

	copy_val(base, &TypeObject::tp_dictoffset);

	// FIXME: doesn't work because 'contains' requires 'tp_cmp' and 'tp_hash' to be implemented first
	//if (LIST mro{ base->tp_mro }) {
	//	if (mro.contains(api::typeof<TYPE>())) { flag_set(tp_flags, TypeFlags_Type_Subclass); }
	//	else if (mro.contains(api::typeof<INT>())) { flag_set(tp_flags, TypeFlags_Int_Subclass); }
	//	else if (mro.contains(api::typeof<FLT>())) { flag_set(tp_flags, TypeFlags_Float_Subclass); }
	//	else if (mro.contains(api::typeof<STR>())) { flag_set(tp_flags, TypeFlags_Str_Subclass); }
	//	else if (mro.contains(api::typeof<LIST>())) { flag_set(tp_flags, TypeFlags_List_Subclass); }
	//	else if (mro.contains(api::typeof<DICT>())) { flag_set(tp_flags, TypeFlags_Dict_Subclass); }
	//}
}

void TypeObject::inherit_slots(TypeObject * base)
{
	TypeObject * basebase{ *(base->tp_base) };

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

	if (!tp_cmp && !tp_hash)
	{
		STR_IDENTIFIER(__eq__);
		STR_IDENTIFIER(__hash__);
		if (DICT dict{ tp_dict }; dict && !dict.contains(&ID___eq__) && !dict.contains(&ID___hash__))
		{
			tp_cmp = base->tp_cmp;
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

OBJ ism::api::type_getattro(TYPE type, OBJ name)
{
	return generic_getattr_with_dict(type, name, nullptr);
}

Error ism::api::type_setattro(TYPE type, OBJ name, OBJ value)
{
	VERIFY(STR::check_(name));

	Error err{ generic_setattr_with_dict(type, name, value, nullptr) };

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