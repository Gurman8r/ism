#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// renderer storage
	class ISM_API RendererStorage : public Object
	{
		OBJECT_COMMON(RendererStorage, Object);

		friend class RenderingServerDefault;

		RenderingDevice * const m_device;

	protected:
		explicit RendererStorage(RenderingDevice * device);

	public:
		virtual ~RendererStorage();

	public:
		/* TEXTURE */
		RID texture2d_placeholder_create();
		RID texture3d_placeholder_create();
		RID texturecube_placeholder_create();

	public:
		/* SHADER */
		RID shader_placeholder_create();

	public:
		/* MATERIAL */
		struct _Material
		{
			RID shader{};
			RID uniform_buffer{};
			RID uniform_set{};
			Vector<RID> texture_cache{};
			DynamicBuffer ubo_data{};
			Map<StringName, Variant> params{};
			bool update_requested{};
		};

		RID material_create();
		RID material_placeholder_create();
		void material_destroy(RID material);
		RID material_get_shader(RID material);
		void material_set_shader(RID material, RID shader);
		Variant material_get_param(RID material, StringName const & key);
		void material_set_param(RID material, StringName const & key, Variant const & value);
		void material_update(RID material, Map<StringName, Variant> const & params);

	public:
		/* MESH */
		struct _Mesh
		{
			struct Surface
			{
				RS::Primitive_ primitive{};

				RID vertex_array{};
				size_t vertex_count{};
				size_t vertex_buffer_size{};

				RID index_array{};
				size_t index_count{};
				size_t index_buffer_size{};

				RID material{};
				RID uniform_set{};
			};

			Vector<Surface *> surfaces{};
		};

		RID mesh_create(Vector<RS::SurfaceData> const & surfaces = {});
		RID mesh_placeholder_create();
		void mesh_destroy(RID mesh);
		void mesh_clear(RID mesh);
		void mesh_add_surface(RID mesh, RS::SurfaceData const & surf);
		size_t mesh_get_surface_count(RID mesh);
		RS::Primitive_ mesh_surface_get_primitive(RID mesh, size_t index);
		RID mesh_surface_get_vertex_array(RID mesh, size_t index);
		RID mesh_surface_get_index_array(RID mesh, size_t index);
		RID mesh_surface_get_material(RID mesh, size_t index);

	public:
		/* RENDER TARGET */
		struct _RenderTarget
		{
			Vec2i size{};
			RID framebuffer{}, backbuffer{};
			RID color{};
			RD::DataFormat_ color_format{}, color_format_srgb{};
			RID texture{};
			RID framebuffer_uniforms{}, backbuffer_uniforms{};
			bool clear_requested{};
			Color clear_color{};
		};

		RID render_target_create();
		void render_target_destroy(RID render_target);
		Vec2i render_target_get_position(RID render_target);
		void render_target_set_position(RID render_target, int32_t x, int32_t  y);
		Vec2i render_target_get_size(RID render_target);
		void render_target_set_size(RID render_target, int32_t width, int32_t height);
		RID render_target_get_texture(RID render_target);
		void render_target_request_clear(RID render_target, Color const & value);
		void render_target_disable_clear_request(RID render_target);
		bool render_target_is_clear_requested(RID render_target);
		Color render_target_get_clear_request_color(RID render_target);
		void render_target_do_clear_request(RID render_target);

	public:
		/* CAMERA */
		struct _Camera
		{
			Mat4 xform{};
		};

		RID camera_create();
		void camera_set_perspective(RID camera, float_t fovy_degrees, float_t z_near, float_t z_far);
		void camera_set_orthogonal(RID camera, float_t size, float_t z_near, float_t z_far);
		void camera_set_frustum(RID camera, float_t size, Vec2 offset, float_t z_near, float_t z_far);
		void camera_set_transform(RID camera, Mat4 const & transform);
		void camera_set_cull_mask(RID camera, uint32_t layers);
		void camera_set_environment(RID camera, RID env);
		void camera_set_camera_effects(RID camera, RID fx);
		void camera_set_use_vertical_aspect(RID camera, bool enable);

	public:
		/* VIEWPORT */
		struct _Viewport
		{
			RID self{}, parent{};
			Vec2i size{};
			RID camera{};
			RID render_target{}, render_target_texture{};
		};

		RID viewport_create();
		void viewport_destroy(RID viewport);
		void viewport_set_parent_viewport(RID viewport, RID parent_viewport);
		void viewport_set_size(RID viewport, int32_t width, int32_t height);
		RID viewport_get_texture(RID viewport) const;
		void viewport_attach_to_screen(RID viewport, IntRect const & rect, DS::WindowID screen);
		void draw_viewports();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
