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

	protected:
		explicit RendererStorage();

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
		struct Material
		{
			RID shader{};
			RID uniform_buffer{};
			RID uniform_set{};
			Vector<RID> texture_cache{};
			DynamicBuffer ubo_data{};
			Map<StringName, UniformVariant> params{};
			bool update_requested{};
		};

		RID material_create();
		RID material_placeholder_create();
		void material_destroy(RID material);
		RID material_get_shader(RID material);
		void material_set_shader(RID material, RID shader);
		UniformVariant material_get_param(RID material, StringName const & key);
		void material_set_param(RID material, StringName const & key, UniformVariant const & value);
		void material_update_uniform_buffer(RID material, Map<StringName, UniformVariant> const & params);
		void material_update_textures(RID material, Map<StringName, UniformVariant> const & params, Map<StringName, RID> const & default_textures, Vector<String> const & texture_uniforms, Vector<RID> const & textures);
		void material_update_parameters(RID material, Map<StringName, UniformVariant> const & params, bool uniforms_dirty, bool textures_dirty);

	public:
		/* MESH */
		struct Mesh
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
		struct RenderTarget
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
		struct Camera
		{
			Vec3 position{};
			Vec4 rotation{}; // (quaternion)
			Mat4 xform{};
		};

		RID camera_create(Vec3 const & position = {}, Vec4 const & rotation = {});
		void camera_destroy(RID camera);
		Vec3 camera_get_position(RID camera);
		void camera_set_position(RID camera, Vec3 const & value);
		Vec4 camera_get_rotation(RID camera);
		void camera_set_rotation(RID camera, Vec4 const & value);
		Mat4 camera_get_transform(RID camera);

	public:
		struct Viewport
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
		void viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen);
		void draw_viewports();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
