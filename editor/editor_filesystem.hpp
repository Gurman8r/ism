#ifndef _ISM_EDITOR_FILESYSTEM_HPP_
#define _ISM_EDITOR_FILESYSTEM_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	// filesystem component of the main editor
	class ISM_EDITOR_API EditorFileSystem : public EditorPanel
	{
		DEFINE_CLASS(EditorFileSystem, EditorPanel);

	public:
		EditorFileSystem();

		virtual ~EditorFileSystem();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_FILESYSTEM_HPP_
