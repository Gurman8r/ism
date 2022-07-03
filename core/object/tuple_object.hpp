#ifndef _ISM_TUPLE_OBJECT_HPP_
#define _ISM_TUPLE_OBJECT_HPP_

// PLACEHOLDER

#include <core/object/type_object.hpp>

namespace ism
{
	// tuple object
	class TupleObject : public BaseObject
	{
		OBJECT_COMMON(TupleObject, BaseObject);

		friend class TUPLE;

	public:
	};

	// tuple delete
	template <> struct DefaultDelete<TupleObject> : DefaultDelete<BaseObject> {};

	// tuple check
#define OBJECT_CHECK_TUPLE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Tuple_Subclass))

	// tuple ref
	class TUPLE : public Ref<TupleObject>
	{
		REF_COMMON(TUPLE, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_ISM_TUPLE_OBJECT_HPP_
