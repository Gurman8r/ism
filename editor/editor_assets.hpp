#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_FILESYSTEM_HPP_
#define _ISM_EDITOR_FILESYSTEM_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	// filesystem component of the main editor
	class ISM_API EditorAssets : public EditorPanel
	{
		OBJECT_CLASS(EditorAssets, EditorPanel);

	public:
		EditorAssets();

		virtual ~EditorAssets();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_FILESYSTEM_HPP_
#endif // TOOLS_ENABLED
