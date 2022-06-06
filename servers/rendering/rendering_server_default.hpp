#ifndef _ISM_RENDERING_SERVER_DEFAULT_HPP_
#define _ISM_RENDERING_SERVER_DEFAULT_HPP_

#include <servers/rendering/renderer_storage.hpp>
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
		RendererCanvasRenderer * m_canvas_renderer{};
		RendererSceneRenderer * m_scene_renderer{};

	public:
		RenderingServerDefault();
		virtual ~RenderingServerDefault() override;

	public:
		virtual void initialize() override;
		virtual void finalize() override;

	public:
		/* TEXTURE */
		virtual void texture_destroy(RID texture) override;
		virtual RID texture_replace(RID old_texture, RID new_texture) override;
		virtual RID texture2d_create(Ref<Image> const & image) override;
		virtual RID texture2d_placeholder_create() override;
		virtual Ref<Image> texture2d_get_image(RID texture) override;

	public:
		/* SHADER */
		virtual RID shader_create() override;
		virtual RID shader_placeholder_create() override;
		virtual void shader_destroy(RID shader) override;

	public:
		/* MATERIAL */
		virtual RID material_create() override;
		virtual RID material_placeholder_create() override;
		virtual void material_destroy(RID material) override;
		virtual RID material_get_shader(RID material) const override;
		virtual void material_set_shader(RID material, RID shader) override;
		virtual OBJ material_get_param(RID material, StringName const & key) const override;
		virtual void material_set_param(RID material, String const & key, OBJ const & value) override;

	public:
		/* MESH */
		virtual RID mesh_create(Vector<SurfaceData> const & surfaces = {}) override;
		virtual RID mesh_placeholder_create() override;
		virtual void mesh_destroy(RID mesh) override;
		virtual void mesh_clear(RID mesh) override;
		virtual void mesh_add_surface(RID mesh, SurfaceData const & surface) override;
		virtual size_t mesh_get_surface_count(RID mesh) override;
		virtual RS::Primitive_ mesh_surface_get_primitive(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_vertex_array(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_index_array(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_material(RID mesh, size_t index) override;

	public:
		/* CAMERA */
		virtual RID camera_create() override;
		virtual void camera_destroy(RID camera) override;

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
