#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

// WIP

#include <core/object/type_object.hpp>

namespace ism
{
	// generic object
	class GenericObject : public Object
	{
		OBJECT_COMMON(GenericObject, Object);

		friend class GENERIC;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<Object> {};

	// generic check
#define OBJECT_CHECK_TUPLE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Tuple_Subclass))

	// generic ref
	class GENERIC : public Ref<GenericObject>
	{
		REF_COMMON(GENERIC, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
