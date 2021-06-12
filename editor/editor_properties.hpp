#ifndef _ISM_EDITOR_PROPERTIES_HPP_
#define _ISM_EDITOR_PROPERTIES_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API EditorProperties : public Super
	{
		ISM_SUPER_CLASS(EditorProperties, Super);

	public:
		virtual ~EditorProperties() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(EditorProperties);
	};
}

#endif // !_ISM_EDITOR_PROPERTIES_HPP_
