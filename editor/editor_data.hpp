#ifndef _ISM_EDITOR_DATA_HPP_
#define _ISM_EDITOR_DATA_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API EditorData
	{
	public:
		virtual ~EditorData();

		DEFAULT_COPYABLE_MOVABLE(EditorData);
	};
}

#endif // !_ISM_EDITOR_DATA_HPP_
