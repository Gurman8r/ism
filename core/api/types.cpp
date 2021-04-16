#include <core/api/types.hpp>
#include <core/api/internals.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreObject_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreObject);
	t.tp_name = "object";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { FATAL("constructor not implemented"); return nullptr; };

	t.tp_getattr = (getattrfunc)_get_attr_string;
	t.tp_setattr = (setattrfunc)_set_attr_string;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreType_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreType);
	t.tp_name = "type";
	t.tp_basicsize = sizeof(CoreType);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_create = (createfunc)[]() { return TYPE::create(); };

	t.tp_dictoffset = offsetof(CoreType, tp_dict);
	t.tp_weaklistoffset = offsetof(CoreType, tp_weaklist);
	t.tp_vectorcalloffset = offsetof(CoreType, tp_vectorcall);

	t.tp_as_number = COMPOSE(NumberMethods, n) {};
	t.tp_as_sequence = COMPOSE(SequenceMethods, s) {};
	t.tp_as_mapping = COMPOSE(MappingMethods, m) {};

	t.tp_getattr = (getattrfunc)_get_attr_string;
	t.tp_setattr = (setattrfunc)_set_attr_string;

	t.tp_compare = (cmpfunc)[](TYPE self, OBJECT v)
	{
		if (isinstance<TYPE>(v)) return util::compare(self->tp_name, TYPE(v)->tp_name);
		return util::compare(self.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](TYPE o) { return hash(o->tp_name); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](TYPE o) { return STR::create(o->tp_name); };
	t.tp_str = (reprfunc)[](TYPE o) { return STR::create(o->tp_name); };

	t.tp_getsets =
	{
		GetSetDef{ "__basicsize__", (getter)[](TYPE o, auto) { return INT::create(o->tp_basicsize); }, },
		GetSetDef{ "__flags__", (getter)[](TYPE o, auto) { return INT::create(o->tp_flags); }, },
		GetSetDef{ "__base__", (getter)[](TYPE o, auto) { return o->tp_base; }, },
		GetSetDef{ "__mro__", (getter)[](TYPE o, auto) { return o->tp_mro; }, },
		GetSetDef{ "__dict__", (getter)[](TYPE o, auto) { return o->tp_dict; }, },
		GetSetDef{ "__weaklist__", (getter)[](TYPE o, auto) { return o->tp_weaklist; }, },
		
		GetSetDef{ "__name__",
			(getter)[](TYPE o, auto) { return STR::create(o->tp_name); },
			(setter)[](TYPE o, STR v, auto) { o->tp_name = ***v; return Error_None; },
		},
		GetSetDef{ "__qualname__",
			(getter)[](TYPE o, auto) { return nullptr; },
			(setter)[](TYPE o, OBJECT v, auto) { return Error_None; },
		},
		GetSetDef{ "__bases__",
			(getter)[](TYPE o, auto) { return o->tp_bases; },
			(setter)[](TYPE o, OBJECT v, auto) { return (o->tp_bases = v), Error_None; },
		},
		GetSetDef{ "__module__",
			(getter)[](TYPE o, auto) { return o->tp_dict["__module__"]; },
			(setter)[](TYPE o, OBJECT v, auto) { return (o->tp_dict["__module__"] = v), Error_None; },
		},
		GetSetDef{ "__abstractmethods__",
			(getter)[](TYPE o, auto) { return o->tp_dict["__abstractmethods__"]; },
			(setter)[](TYPE o, OBJECT v, auto) { return (o->tp_dict["__abstractmethods__"] = v), Error_None; },
		},
		GetSetDef{ "__doc__",
			(getter)[](TYPE o, auto) { return STR::create(o->tp_doc); },
			(setter)[](TYPE o, STR v, auto) { return (o->tp_doc = ***v), Error_None; },
		},
		GetSetDef{ "__text_signature__",
			(getter)[](TYPE o, auto) { return nullptr; },
		},
	};

	t.tp_methods =
	{
		MethodDef{ "mro",
			(cfunction)[](TYPE o, auto args) { return o->tp_mro; },
			MethodFlags_None
		},
		MethodDef{ "__subclasses__",
			(cfunction)[](TYPE o, auto args) { return o->tp_subclasses; },
			MethodFlags_None
		},
		MethodDef{ "__instancecheck__",
			(cfunction)[](TYPE o, auto args) { return nullptr; },
			MethodFlags_None
		},
		MethodDef{ "__subclasscheck__",
			(cfunction)[](TYPE o, auto args) { return nullptr; },
			MethodFlags_None
		},
		MethodDef{ "__dir__",
			(cfunction)[](TYPE o, auto args) { return nullptr; },
			MethodFlags_None
		},
		MethodDef{ "__sizeof__",
			(cfunction)[](TYPE o, auto args) { return nullptr; },
			MethodFlags_None
		},
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreNone_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreObject);
	t.tp_name = "none";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_base = OBJECT::type_static();
	t.tp_flags = TypeFlags_Default;
	t.tp_doc = "";
};

CoreObject ism::_Core_None{ CoreNone_Type };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreBool_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "bool";
	t.tp_basicsize = sizeof(CoreInt);
	t.tp_base = INT::type_static();
	t.tp_as_number = COMPOSE(NumberMethods, m) {};
};

CoreInt ism::_Core_True = COMPOSE(CoreInt, v)
{
	v.ob_type = CoreBool_Type;
	v.m_data = true;
};

CoreInt ism::_Core_False = COMPOSE(CoreInt, v)
{
	v.ob_type = CoreBool_Type;
	v.m_data = false;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreInt_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreInt);
	t.tp_name = "int";
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;
	t.tp_create = (createfunc)[]() { return INT::create(); };

	t.tp_compare = (cmpfunc)[](INT o, OBJECT v)
	{
		if (isinstance<INT>(v)) return util::compare(***o, ***INT(v));
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](INT o) { return Hash<ssize_t>()(***o); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](INT o) { return STR::create(to_string(***o)); };
	t.tp_str = (reprfunc)[](INT o) { return STR::create(to_string(***o)); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreFloat_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreFloat);
	t.tp_name = "float";
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;
	t.tp_create = (createfunc)[]() { return FLT::create(); };

	t.tp_compare = (cmpfunc)[](FLT o, OBJECT v)
	{
		if (isinstance<FLT>(v)) return util::compare(***o, ***FLT(v));
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](FLT o) { return Hash<double_t>()(***o); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](FLT o) { return STR::create(to_string(***o)); };
	t.tp_str = (reprfunc)[](FLT o) { return STR::create(to_string(***o)); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreString_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreString);
	t.tp_name = "str";
	t.tp_basicsize = sizeof(CoreString);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	t.tp_create = (createfunc)[]() { return STR::create(); };

	t.tp_compare = (cmpfunc)[](STR o, OBJECT v)
	{
		if (isinstance<STR>(v)) return util::compare(***o, ***STR(v));
		else if (STR str{ v.str() }) return util::compare(***o, ***str);
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](STR o) { return hash(***o); };
	t.tp_len = (lenfunc)[](STR o) { return o->size(); };
	t.tp_repr = (reprfunc)[](STR o) { return o; };
	t.tp_str = (reprfunc)[](STR o) { return o; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreList_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreList);
	t.tp_name = "list";
	t.tp_basicsize = sizeof(CoreList);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;
	t.tp_create = (createfunc)[]() { return LIST::create(); };

	t.tp_compare = (cmpfunc)[](LIST o, OBJECT v)
	{
		if (isinstance<LIST>(v)) return util::compare(***o, ***LIST(v));
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](LIST o) { return o->size(); };

	t.tp_methods =
	{
		MethodDef{ "__contains__", (cfunction)[](LIST o, OBJECT v) { return Core_False; } },
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreDict_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreDict);
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(CoreDict);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_create = (createfunc)[]() { return DICT::create(); };

	t.tp_compare = (cmpfunc)[](DICT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](DICT o) { return o->size(); };

	t.tp_methods =
	{
		MethodDef{ "__contains__",
			(cfunction)[](DICT o, OBJECT v) { return Core_Boolean(o && v && (***o).end() != (***o).find(v)); },
		},
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreCapsule_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreCapsule);
	t.tp_name = "capsule";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return CAPSULE::create(); };

	t.tp_compare = (cmpfunc)[](CAPSULE o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ism::_CoreFunction_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreFunction);
	t.tp_name = "function";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return FUNCTION::create(); };

	t.tp_dictoffset = offsetof(CoreFunction, m_dict);
	t.tp_weaklistoffset = offsetof(CoreFunction, m_weakreflist);
	t.tp_vectorcalloffset = offsetof(CoreFunction, m_vectorcall);

	t.tp_compare = (cmpfunc)[](FUNCTION o, OBJECT v)	
	{
		return util::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */