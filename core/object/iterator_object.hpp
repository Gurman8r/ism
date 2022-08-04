#ifndef _ISM_ITERATOR_OBJECT_HPP_
#define _ISM_ITERATOR_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/type_object.hpp>

namespace ism
{
	// iterator object
	class IteratorObject : public Object
	{
		OBJECT_CLASS(IteratorObject, Object);

		friend class ITERATOR;

	public:
	};

	// iterator delete
	template <> struct DefaultDelete<IteratorObject> : DefaultDelete<Object> {};

	// iterator check
#define OBJECT_CHECK_TUPLE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Tuple_Subclass))

	// iterator ref
	class ITERATOR : public Ref<IteratorObject>
	{
		REF_CLASS(ITERATOR, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_ISM_ITERATOR_OBJECT_HPP_
