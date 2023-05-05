#ifndef _ISM_TUPLE_OBJECT_HPP_
#define _ISM_TUPLE_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Ism
{
	// tuple object
	class TupleObject : public Object
	{
		DEFINE_CLASS(TupleObject, Object);

		friend class TupleRef;

	public:
	};

	// tuple delete
	template <> struct DefaultDelete<TupleObject> : DefaultDelete<Object> {};

	// tuple check
#define OBJECT_CHECK_TUPLE(o) (Ism::typeof(o).has_feature(Ism::TypeFlags_Tuple_Subclass))

	// tuple ref
	class TupleRef : public Ref<TupleObject>
	{
		CUSTOM_REF(TupleRef, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_ISM_TUPLE_OBJECT_HPP_
