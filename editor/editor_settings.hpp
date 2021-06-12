#ifndef _ISM_EDITOR_SETTINGS_HPP_
#define _ISM_EDITOR_SETTINGS_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API EditorSettings : public Super
	{
		ISM_SUPER_CLASS(EditorSettings, Super);

	public:
		virtual ~EditorSettings() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(EditorSettings);
	};
}

#endif // !_ISM_EDITOR_SETTINGS_HPP_
