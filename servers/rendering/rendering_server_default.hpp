#ifndef _ISM_RENDERING_SERVER_DEFAULT_HPP_
#define _ISM_RENDERING_SERVER_DEFAULT_HPP_

#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering/renderer_viewport.hpp>
#include <servers/rendering/renderer_canvas_renderer.hpp>
#include <servers/rendering/renderer_scene_renderer.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServerDefault : public RenderingServer
	{
		OBJECT_COMMON(RenderingServerDefault, RenderingServer);

	protected:
		RenderingDevice * m_device{};
		RendererStorage * m_storage{};
		RendererViewport * m_viewport{};
		RendererCanvasRenderer * m_canvas_renderer{};
		RendererSceneRenderer * m_scene_renderer{};

	public:
		RenderingServerDefault();
		virtual ~RenderingServerDefault() override;

		virtual void initialize() override;
		
		virtual void finalize() override;

	public:
		/* TEXTURE */
		virtual RID texture_replace(RID old_texture, RID new_texture) override;
		
		virtual RID texture2d_create(Ref<Image> const & image) override;
		
		virtual RID texture2d_placeholder_create() override;
		
		virtual Ref<Image> texture2d_get_image(RID texture) override;

	public:
		/* SHADER */
		virtual RID shader_create() override;

	public:
		/* MATERIAL */
		virtual RID material_create() override;

		virtual void material_set_shader(RID material, RID shader) override;

	public:
		/* MESH */
		virtual RID mesh_create() override;

	public:
		/* CAMERA */
		virtual RID camera_create() override;

	public:
		/* VIEWPORT */
		virtual RID viewport_create() override;

		virtual void viewport_destroy(RID viewport) override;

		virtual void viewport_set_parent_viewport(RID viewport, RID parent_viewport) override;

		virtual void viewport_set_size(RID viewport, int32_t width, int32_t height) override;

		virtual RID viewport_get_texture(RID viewport) const override;

		virtual void viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_DEFAULT_HPP_
