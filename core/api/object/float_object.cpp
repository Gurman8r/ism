#include <core/api/object/float_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_COMPOSE_TYPE_OBJECT(FloatObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT self) { return Hash<double_t>()(***FLT(self)); };
	
	t.tp_repr = (reprfunc)[](OBJECT self) { return STR(util::to_string(***FLT(self))); };
	
	t.tp_str = (reprfunc)[](OBJECT self) { return STR(util::to_string(***FLT(self))); };

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other)
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

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FloatObject::_bind_class(OBJECT scope)
{
	CLASS_<FLT>(scope, "float", get_type_static())

		.def(init<>())
		
		.def(init<storage_type>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */