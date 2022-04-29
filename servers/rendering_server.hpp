#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image_loader.hpp>
#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// high-level graphics api
	class ISM_API RenderingServer : public Object
	{
		OBJECT_COMMON(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		explicit RenderingServer() noexcept { singleton = this; }

	public:
		virtual ~RenderingServer() override;

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }

		virtual void initialize() = 0;

		virtual void finalize() = 0;

	public:
		/* TEXTURE */
		virtual RID texture_replace(RID old_texture, RID new_texture) = 0;
		virtual RID texture2d_create(Ref<Image> const & image) = 0;
		virtual Ref<Image> texture2d_get(RID texture) = 0;

	public:
		/* RENDERER */
		virtual void begin_scene() = 0;
		virtual void end_scene() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// rendering server singleton
#define RS (ism::RenderingServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
