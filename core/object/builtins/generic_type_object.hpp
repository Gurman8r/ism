#ifndef _ISM_GENERIC_TYPE_OBJECT_HPP_
#define _ISM_GENERIC_TYPE_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Ism
{
	// generic object
	class GenericTypeObject : public Object
	{
		OBJECT_CLASS(GenericTypeObject, Object);

		friend class GenericTypeRef;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericTypeObject> : DefaultDelete<Object> {};

	// generic check
#define OBJECT_CHECK_GENERIC(o) (isinstance<GenericTypeRef>(o))

	// generic ref
	class GenericTypeRef : public Ref<GenericTypeObject>
	{
		CUSTOM_REF(GenericTypeRef, OBJECT_CHECK_GENERIC);

	public:
	};
}

#endif // !_ISM_GENERIC_TYPE_OBJECT_HPP_
