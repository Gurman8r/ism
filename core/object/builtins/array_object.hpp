#ifndef _ISM_ARRAY_OBJECT_HPP_
#define _ISM_ARRAY_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Ism
{
	// array object
	class ArrayObject : public Object
	{
		OBJECT_CLASS(ArrayObject, Object);

		friend class ArrayRef;

	public:
	};

	// array delete
	template <> struct DefaultDelete<ArrayObject> : DefaultDelete<Object> {};

	// array check
#define OBJECT_CHECK_ARRAY(o) (Ism::typeof(o).has_feature(Ism::TypeFlags_Array_Subclass))

	// array ref
	class ArrayRef : public Ref<ArrayObject>
	{
		CUSTOM_REF(ArrayRef, OBJECT_CHECK_ARRAY);

	public:
	};
}

#endif // !_ISM_ARRAY_OBJECT_HPP_
