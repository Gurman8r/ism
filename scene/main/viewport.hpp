#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Viewport : public Node
	{
		ISM_SUPER(Viewport, Node);

	public:
		virtual ~Viewport() override;

		explicit Viewport(Node * parent, SceneTree * tree) : Node{ parent, tree } {}

		explicit Viewport(SceneTree * tree) : Node{ tree } {}

		explicit Viewport(Node * parent) : Node{ parent } {}

		NON_COPYABLE(Viewport);

		MOVABLE(Viewport);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
