#ifndef _ISM_EDITOR_DATA_HPP_
#define _ISM_EDITOR_DATA_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API EditorData : public Super
	{
		ISM_SUPER_CLASS(EditorData, Super);

	public:
		virtual ~EditorData() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(EditorData);
	};
}

#endif // !_ISM_EDITOR_DATA_HPP_
