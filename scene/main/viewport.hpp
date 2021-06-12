#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Viewport : public Node
	{
		ISM_SUPER_CLASS(Viewport, Node);

	private:

	public:
		virtual ~Viewport() override = default;
		
		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(Viewport);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
