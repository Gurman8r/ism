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

		static RendererStorage * singleton;

		friend class RenderingServerDefault;

	protected:
		explicit RendererStorage();

	public:
		virtual ~RendererStorage();

		FORCE_INLINE static RendererStorage * get_singleton() noexcept { return singleton; }

	public:
		/* TEXTURE */
		RID texture2d_placeholder_create();

	public:
		/* SHADER */
		enum ShaderType_
		{
			ShaderType_2D,
			ShaderType_3D,
			ShaderType_Particles,
			ShaderType_Sky,
			ShaderType_MAX
		};

	public:
		/* MATERIAL */
		struct Material
		{
			RID shader{};
			RID uniform_buffer{};
			RID uniform_set{};
			Vector<RID> texture_cache{};
			DynamicBuffer ubo_data{};
		};

		RID material_create();
		void material_destroy(RID material);
		void material_set_shader(RID material, RID shader);
		void material_set_param(RID material, String const & key, OBJ const & value);
		OBJ material_get_param(RID material, String const & key);

	public:
		/* MESH */
		struct Mesh
		{
			struct Surface
			{
				RD::PrimitiveType_ primitive{};

				RID vertex_array{};
				size_t vertex_count{};
				size_t vertex_buffer_size{};

				RID index_array{};
				size_t index_count{};

				RID material{};
				RID uniform_set{};
			};

			Vector<Surface *> surfaces{};
		};

		RID mesh_create(Vector<RS::SurfaceData> const & surfaces = {});
		void mesh_destroy(RID mesh);
		void mesh_clear(RID mesh);
		void mesh_add_surface(RID mesh, RS::SurfaceData const & surf);
		size_t mesh_get_surface_count(RID mesh);

		RD::PrimitiveType_ mesh_surface_get_primitive(RID mesh, size_t index);
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

		void render_target_set_position(RID render_target, int32_t x, int32_t  y);
		void render_target_set_size(RID render_target, int32_t width, int32_t height);

		RID render_target_get_texture(RID render_target);
		Vec2i render_target_get_position(RID render_target);
		Vec2i render_target_get_size(RID render_target);

		void render_target_request_clear(RID render_target, Color const & value);
		void render_target_disable_clear_request(RID render_target);
		bool render_target_is_clear_requested(RID render_target);
		Color render_target_get_clear_request_color(RID render_target);
		void render_target_do_clear_request(RID render_target);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define RENDERER_STORAGE (ism::RendererStorage::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
