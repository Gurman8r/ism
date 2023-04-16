#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/impl/type_object.hpp>

namespace ism
{
	// generic object
	class GenericObject : public Object
	{
		DEFINE_CLASS(GenericObject, Object);

		friend class GENERIC;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<Object> {};

	// generic check
#define OBJECT_CHECK_GENERIC(o) (isinstance<GENERIC>(o))

	// generic ref
	class GENERIC : public Ref<GenericObject>
	{
		REF_CLASS(GENERIC, OBJECT_CHECK_GENERIC);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
