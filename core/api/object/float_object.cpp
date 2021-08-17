#include <core/api/object/float_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(FloatObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<double_t>()(***FLT(self)); };
	
	t.tp_repr = (reprfunc)[](OBJ self) { return STR(util::to_string(***FLT(self))); };
	
	t.tp_str = (reprfunc)[](OBJ self) { return STR(util::to_string(***FLT(self))); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (FLT::check_(other))
		{
			return util::compare(***FLT(self), ***FLT(other));
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FloatObject::_bind_class(OBJ scope)
{
	CLASS_<FLT>(scope, "float", get_class())

		.def(init<>())
		
		.def(init<storage_type>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */