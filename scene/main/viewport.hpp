#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <servers/rendering_server.hpp>

namespace ism
{
	class ISM_API Viewport : public Node
	{
		OBJECT_COMMON(Viewport, Node);

	protected:
		Viewport();

	public:
		virtual ~Viewport() override;
	};
}

#endif // !_ISM_VIEWPORT_HPP_
