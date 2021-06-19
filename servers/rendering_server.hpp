#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	class ISM_API RenderingServer : public Super
	{
		ISM_SUPER(RenderingServer, Super);

	public:
		virtual ~RenderingServer();

		DEFAULT_COPYABLE_MOVABLE(RenderingServer);
	};
}

#endif // !_ISM_RENDERING_SERVER_HPP_
