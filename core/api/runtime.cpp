#include <core/api/runtime.hpp>

using namespace ISM;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreObject_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreObject);
	t.tp_name = "object";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { FATAL("constructor not implemented"); return nullptr; };

	t.tp_getattr = (getattrfunc)[](OBJECT o, cstring i) -> OBJECT
	{
		if (auto def{ GetSetDef::find_in(o.type()->tp_getsets, i) }; def && def->get)
		{
			return def->get(o, def);
		}
		return nullptr;
	};

	t.tp_setattr = (setattrfunc)[](OBJECT o, cstring i, OBJECT v) -> Err
	{
		if (auto def{ GetSetDef::find_in(o.type()->tp_getsets, i) }; def && def->set)
		{
			def->set(o, v, def);
			return Err_None;
		}
		return Err_Unknown;
	};
};

void CoreObject::_postinitialize() { predelete_handler((Super *)this); }

bool CoreObject::_predelete() { return predelete_handler((Super *)this); }

CoreObject::~CoreObject() {}

CoreObject::CoreObject(TYPE const & t) : Reference{}, ob_type{ t } {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreType_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreType);
	t.tp_name = "type";
	t.tp_basicsize = sizeof(CoreType);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;
	t.tp_create = (createfunc)[]() { return TYPE{}.instance(); };

	t.tp_dictoffset = offsetof(CoreType, tp_dict);
	t.tp_weaklistoffset = offsetof(CoreType, tp_weaklist);
	t.tp_vectorcalloffset = offsetof(CoreType, tp_vectorcall);

	t.tp_as_number = COMPOSE(AsNumber, n) {};
	t.tp_as_sequence = COMPOSE(AsSequence, s) {};
	t.tp_as_mapping = COMPOSE(AsMapping, m) {};

	t.tp_getattr = _CoreObject_Type.tp_getattr;
	t.tp_setattr = _CoreObject_Type.tp_setattr;

	t.tp_compare = (cmpfunc)[](TYPE self, OBJECT v)
	{
		if (isinstance<TYPE>(v)) return UTIL::compare(self->tp_name, TYPE(v)->tp_name);
		return UTIL::compare(self.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](TYPE o) { return hashof(o->tp_name); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](TYPE o) { return STR{}.instance(o->tp_name); };
	t.tp_str = (reprfunc)[](TYPE o) { return STR{}.instance(o->tp_name); };

	t.tp_getsets =
	{
		GetSetDef{ "__basicsize__", (getter)[](TYPE o, auto) { return INT{}.instance(o->tp_basicsize); }, },
		GetSetDef{ "__flags__", (getter)[](TYPE o, auto) { return INT{}.instance(o->tp_flags); }, },
		GetSetDef{ "__base__", (getter)[](TYPE o, auto) { return o->tp_base; }, },
		GetSetDef{ "__mro__", (getter)[](TYPE o, auto) { return o->tp_mro; }, },
		GetSetDef{ "__dict__", (getter)[](TYPE o, auto) { return o->tp_dict; }, },
		GetSetDef{ "__weaklist__", (getter)[](TYPE o, auto) { return o->tp_weaklist; }, },
		
		GetSetDef{ "__name__",
			(getter)[](TYPE o, auto) { return STR{}.instance(o->tp_name); },
			(setter)[](TYPE o, STR v, auto) { o->tp_name = ***v; return Err_None; },
		},
		GetSetDef{ "__qualname__",
			(getter)[](TYPE o, auto) { return nullptr; },
			(setter)[](TYPE o, OBJECT v, auto) { return Err_None; },
		},
		GetSetDef{ "__bases__",
			(getter)[](TYPE o, auto) { return o->tp_bases; },
			(setter)[](TYPE o, OBJECT v, auto) { o->tp_bases = v; return Err_None; },
		},
		GetSetDef{ "__module__",
			(getter)[](TYPE o, auto) { return o->tp_dict["__module__"]; },
			(setter)[](TYPE o, OBJECT v, auto) { o->tp_dict["__module__"] = v; return Err_None; },
		},
		GetSetDef{ "__abstractmethods__",
			(getter)[](TYPE o, auto) { return o->tp_dict["__abstractmethods__"]; },
			(setter)[](TYPE o, OBJECT v, auto) { o->tp_dict["__abstractmethods__"] = v; return Err_None; },
		},
		GetSetDef{ "__doc__",
			(getter)[](TYPE o, auto) { return STR{}.instance(o->tp_doc); },
			(setter)[](TYPE o, STR v, auto) { o->tp_doc = ***v; return Err_None; },
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

CoreType::~CoreType() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreNone_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreObject);
	t.tp_name = "none";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_base = OBJECT::type_static();
	t.tp_flags = TypeFlags_Default;
	t.tp_doc = "";
};

CoreObject ISM::_Core_None{ CoreNone_Type };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreBool_Type = COMPOSE(CoreType, t)
{
	t.tp_name = "bool";
	t.tp_basicsize = sizeof(CoreInt);
	t.tp_base = INT::type_static();
	t.tp_as_number = COMPOSE(AsNumber, m) {};
};

CoreInt ISM::_Core_True = COMPOSE(CoreInt, v)
{
	v.ob_type = CoreBool_Type;
	v.m_data = true;
};

CoreInt ISM::_Core_False = COMPOSE(CoreInt, v)
{
	v.ob_type = CoreBool_Type;
	v.m_data = false;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreInt_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreInt);
	t.tp_name = "int";
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;
	t.tp_create = (createfunc)[]() { return INT{}.instance(); };

	t.tp_compare = (cmpfunc)[](INT o, OBJECT v)
	{
		if (isinstance<INT>(v)) return UTIL::compare(***o, ***INT(v));
		else return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](INT o) { return Hash<ssize_t>()(***o); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](INT o) { return STR{}.instance(ISM::to_string(***o)); };
	t.tp_str = (reprfunc)[](INT o) { return STR{}.instance(ISM::to_string(***o)); };
};

CoreInt::~CoreInt() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreFloat_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreFloat);
	t.tp_name = "float";
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;
	t.tp_create = (createfunc)[]() { return FLT{}.instance(); };

	t.tp_compare = (cmpfunc)[](FLT o, OBJECT v)
	{
		if (isinstance<FLT>(v)) return UTIL::compare(***o, ***FLT(v));
		else return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](FLT o) { return Hash<double_t>()(***o); };
	t.tp_len = (lenfunc)[](auto) { return -1; };
	t.tp_repr = (reprfunc)[](FLT o) { return STR{}.instance(to_string(***o)); };
	t.tp_str = (reprfunc)[](FLT o) { return STR{}.instance(to_string(***o)); };
};

CoreFloat::~CoreFloat() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreString_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreString);
	t.tp_name = "str";
	t.tp_basicsize = sizeof(CoreString);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	t.tp_create = (createfunc)[]() { return STR{}.instance(); };

	t.tp_compare = (cmpfunc)[](STR o, OBJECT v)
	{
		if (isinstance<STR>(v)) return UTIL::compare(***o, ***STR(v));
		else if (STR str{ v.str() }) return UTIL::compare(***o, ***str);
		else return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_hash = (hashfunc)[](STR o) { return hashof(***o); };
	t.tp_len = (lenfunc)[](STR o) { return o->size(); };
	t.tp_repr = (reprfunc)[](STR o) { return o; };
	t.tp_str = (reprfunc)[](STR o) { return o; };
};

CoreString::~CoreString() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreList_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreList);
	t.tp_name = "list";
	t.tp_basicsize = sizeof(CoreList);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;
	t.tp_create = (createfunc)[]() { return LIST{}.instance(); };

	t.tp_compare = (cmpfunc)[](LIST o, OBJECT v)
	{
		if (isinstance<LIST>(v)) return UTIL::compare(***o, ***LIST(v));
		else return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](LIST o) { return o->size(); };

	t.tp_methods =
	{
		MethodDef{ "__contains__", (cfunction)[](LIST o, OBJECT v) { return Core_False; } },
	};
};

CoreList::~CoreList() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreDict_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreDict);
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(CoreDict);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_create = (createfunc)[]() { return DICT{}.instance(); };

	t.tp_compare = (cmpfunc)[](DICT o, OBJECT v)
	{
		return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](DICT o) { return o->size(); };

	t.tp_methods =
	{
		MethodDef{ "__contains__",
			(cfunction)[](DICT o, OBJECT v) { return Core_Boolean(o && v && (***o).end() != (***o).find(v)); },
		},
	};
};

CoreDict::~CoreDict() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreCapsule_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreCapsule);
	t.tp_name = "capsule";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return CAPSULE{}.instance(); };

	t.tp_compare = (cmpfunc)[](CAPSULE o, OBJECT v)
	{
		return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };
};

CoreCapsule::~CoreCapsule() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreMethod_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreMethod);
	t.tp_name = "cfunction";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return METHOD{}.instance(); };

	t.tp_compare = (cmpfunc)[](METHOD o, OBJECT v)
	{
		return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };
};

CoreMethod::~CoreMethod() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreGeneric_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreGeneric);
	t.tp_name = "cfunction";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return GENERIC{}.instance(); };

	t.tp_compare = (cmpfunc)[](METHOD o, OBJECT v)
	{
		return UTIL::compare(o.ptr(), v.ptr());
	};
};

CoreGeneric::~CoreGeneric() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CoreType ISM::_CoreModule_Type = COMPOSE(CoreType, t)
{
	t.tp_rtti = &typeid(CoreModule);
	t.tp_name = "module";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_create = (createfunc)[]() { return MODULE{}.instance(); };

	t.tp_dictoffset = offsetof(CoreModule, m_data) + offsetof(CoreModule::storage_type, dict);

	t.tp_compare = (cmpfunc)[](MODULE o, OBJECT v)
	{
		if (isinstance<MODULE>(v))
		{
			return (o.ptr() == v.ptr()) ? 0 : UTIL::compare((***o).name, (***MODULE(v)).name);
		}
		return UTIL::compare(o.ptr(), v.ptr());
	};
	t.tp_len = (lenfunc)[](auto) { return -1; };

	t.tp_getsets =
	{
		GetSetDef{ "__dict__", (getter)[](MODULE o, auto) { return o->m_data.dict; }, },

		GetSetDef{ "__doc__",
			(getter)[](MODULE o, auto) { return STR{}.instance(o->m_data.doc); },
			(setter)[](MODULE o, STR v, auto) { o->m_data.doc = ***v; return Err_None; },
		},
		GetSetDef{ "__name__",
			(getter)[](MODULE o, auto) { return STR{}.instance(o->m_data.name); },
			(setter)[](MODULE o, STR v, auto) { o->m_data.name = ***v; return Err_None; },
		},
	};
};

CoreModule::~CoreModule() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */