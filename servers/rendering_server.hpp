#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	API_CLASS(RenderingServer) : public Object
	{
		OBJ_COMMON(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		explicit RenderingServer() noexcept : Object{} { singleton = this; }

	public:
		virtual ~RenderingServer() override {}

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline RenderingServer & get_rendering_server() { return *CHECK(RenderingServer::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
