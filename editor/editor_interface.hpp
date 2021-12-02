#ifndef _ISM_EDITOR_INTERFACE_HPP_
#define _ISM_EDITOR_INTERFACE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	class ISM_API EditorInterface : public Object
	{
		OBJECT_COMMON(EditorInterface, Object);

	public:
		virtual ~EditorInterface();
	};
}

#endif // !_ISM_EDITOR_INTERFACE_HPP_
