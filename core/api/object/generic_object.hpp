#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

#include <core/api/object/module_object.hpp>

// generic object
namespace ism::api
{
	class ISM_API GenericObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(GenericObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
	};
}

// generic delete
namespace ism { template <> struct DefaultDelete<api::GenericObject> : DefaultDelete<api::BaseObject> {}; }

// generic handle
namespace ism::api
{
	template <> class Handle<GenericObject> : public BaseHandle<GenericObject>
	{
		ISM_HANDLE(GenericObject);

	public:
		Handle() = default;

		~Handle() = default;
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
