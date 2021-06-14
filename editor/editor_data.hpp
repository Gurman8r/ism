#ifndef _ISM_EDITOR_DATA_HPP_
#define _ISM_EDITOR_DATA_HPP_

#include <core/api/modsupport.hpp>

namespace ism
{
	class ISM_API EditorData : public Super
	{
		ISM_SUPER(EditorData, Super);

	public:
		virtual ~EditorData() override;

		DEFAULT_COPYABLE_MOVABLE(EditorData);
	};
}

#endif // !_ISM_EDITOR_DATA_HPP_
