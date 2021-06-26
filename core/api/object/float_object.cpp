#include <core/api/object/float_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(FloatObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<double_t>()(***FLT(o)); };
	
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(util::to_string(***FLT(o))); };
	
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(util::to_string(***FLT(o))); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (FLT::check_(v))
		{
			return util::compare(***FLT(o), ***FLT(v));
		}
		else
		{
			return util::compare(*o, *v);
		}
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