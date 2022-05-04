#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image_loader.hpp>
#include <servers/rendering/rendering_device.hpp>

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
		
		virtual RID texture2d_placeholder_create() = 0;
		
		virtual Ref<Image> texture2d_get_image(RID texture) = 0;

	public:
		/* SHADER */
		virtual RID shader_create() = 0;

	public:
		/* MATERIAL */
		virtual RID material_create() = 0;

		virtual void material_set_shader(RID material, RID shader) = 0;

	public:
		/* MESH */
		virtual RID mesh_create() = 0;

	public:
		/* CAMERA */
		virtual RID camera_create() = 0;

	public:
		/* VIEWPORT */
		virtual RID viewport_create() = 0;
		
		virtual void viewport_destroy(RID viewport) = 0;
		
		virtual void viewport_set_parent_viewport(RID viewport, RID parent_viewport) = 0;
		
		virtual void viewport_set_size(RID viewport, int32_t width, int32_t height) = 0;
		
		virtual RID viewport_get_texture(RID viewport) const = 0;
		
		virtual void viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(RS) RenderingServer;

#define RENDERING_SERVER (ism::RenderingServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
