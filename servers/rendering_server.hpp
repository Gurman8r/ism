#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API RenderingServer
	{
	protected:
		static RenderingServer * singleton;

		explicit RenderingServer() noexcept { singleton = this; }

	public:
		static RenderingServer * get_singleton() { return singleton; }
	};
}

#endif // !_ISM_RENDERING_SERVER_HPP_
