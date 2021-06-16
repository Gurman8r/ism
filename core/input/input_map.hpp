#ifndef _ISM_INPUT_MAP_HPP_
#define _ISM_INPUT_MAP_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	class ISM_API InputMap : public api::BaseObject
	{
		ISM_OBJECT_DEFAULT(InputMap, api::BaseObject);

	protected:
		static void _bind_methods(api::TypeObject & t);

	public:
		virtual ~InputMap();
	};
}

#endif // !_ISM_INPUT_MAP_HPP_
