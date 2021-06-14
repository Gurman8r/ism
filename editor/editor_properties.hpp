#ifndef _ISM_EDITOR_PROPERTIES_HPP_
#define _ISM_EDITOR_PROPERTIES_HPP_

#include <core/api/modsupport.hpp>

namespace ism
{
	class ISM_API EditorProperties : public Super
	{
		ISM_SUPER(EditorProperties, Super);

	public:
		virtual ~EditorProperties() override;

		DEFAULT_COPYABLE_MOVABLE(EditorProperties);
	};
}

#endif // !_ISM_EDITOR_PROPERTIES_HPP_
