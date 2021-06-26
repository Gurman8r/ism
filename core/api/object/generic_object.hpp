#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

#include <core/api/object/module_object.hpp>

// generic
namespace ism
{
	// generic object
	class ISM_API GenericObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(GenericObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<BaseObject> {};

	// generic check
#define ISM_GENERIC_CHECK(o) (ism::isinstance<GENERIC>(o))

	// generic handle
	template <> class NOVTABLE Handle<GenericObject> : public BaseHandle<GenericObject>
	{
		ISM_HANDLE_DEFAULT(GenericObject, ISM_GENERIC_CHECK);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
