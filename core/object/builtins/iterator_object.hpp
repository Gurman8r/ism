#ifndef _ISM_ITERATOR_OBJECT_HPP_
#define _ISM_ITERATOR_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Ism
{
	// iterator object
	class IteratorObject : public Object
	{
		OBJECT_CLASS(IteratorObject, Object);

		friend class IteratorRef;

	public:
	};

	// iterator delete
	template <> struct DefaultDelete<IteratorObject> : DefaultDelete<Object> {};

	// iterator check
#define OBJECT_CHECK_TUPLE(o) (Ism::typeof(o).has_feature(Ism::TypeFlags_Tuple_Subclass))

	// iterator ref
	class IteratorRef : public Ref<IteratorObject>
	{
		CUSTOM_REF(IteratorRef, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_ISM_ITERATOR_OBJECT_HPP_
