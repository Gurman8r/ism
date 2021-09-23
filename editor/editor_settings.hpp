#ifndef _ISM_EDITOR_SETTINGS_HPP_
#define _ISM_EDITOR_SETTINGS_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	API_CLASS(EditorSettings)
	{
	public:
		virtual ~EditorSettings();

		DEFAULT_COPYABLE_MOVABLE(EditorSettings);
	};
}

#endif // !_ISM_EDITOR_SETTINGS_HPP_
