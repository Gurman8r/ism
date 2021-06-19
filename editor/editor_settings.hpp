#ifndef _ISM_EDITOR_SETTINGS_HPP_
#define _ISM_EDITOR_SETTINGS_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API EditorSettings : public Super
	{
		ISM_SUPER(EditorSettings, Super);

	public:
		virtual ~EditorSettings();

		DEFAULT_COPYABLE_MOVABLE(EditorSettings);
	};
}

#endif // !_ISM_EDITOR_SETTINGS_HPP_
