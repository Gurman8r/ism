#ifndef _ISM_EDITOR_DATA_HPP_
#define _ISM_EDITOR_DATA_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	API_CLASS(EditorData)
	{
	public:
		virtual ~EditorData();

		DEFAULT_COPYABLE_MOVABLE(EditorData);
	};
}

#endif // !_ISM_EDITOR_DATA_HPP_
