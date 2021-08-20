#include <core/api/object/float_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(FloatObject, t)
{
	t.tp_name = "float";

	t.tp_size = sizeof(FloatObject);

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

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(FloatObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((FloatObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FloatObject::_bind_methods()
{
	CLASS_<FLT>()

		.def(init<>())
		
		.def(init<storage_type>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */