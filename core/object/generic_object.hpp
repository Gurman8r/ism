#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

// PLACEHOLDER

#include <core/object/type_object.hpp>

namespace ism
{
	// generic object
	class GenericObject : public BaseObject
	{
		OBJECT_COMMON(GenericObject, BaseObject);

		friend class GENERIC;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<BaseObject> {};

	// generic check
#define OBJECT_CHECK_GENERIC(o) (isinstance<GENERIC>(o))

	// generic ref
	class GENERIC : public Ref<GenericObject>
	{
		REF_COMMON(GENERIC, OBJECT_CHECK_GENERIC);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
