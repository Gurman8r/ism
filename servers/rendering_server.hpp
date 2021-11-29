#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD RenderingServerSettings final
	{
		DEFAULT_COPYABLE_MOVABLE(RenderingServerSettings);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServer : public Object
	{
		OBJECT_COMMON(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		explicit RenderingServer() noexcept : Object{} { singleton = this; }

	public:
		virtual ~RenderingServer() override {}

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }

		virtual void initialize() = 0;

		virtual void finalize() = 0;

	public:
		// HIGH LEVEL RENDERING API GOES HERE
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
