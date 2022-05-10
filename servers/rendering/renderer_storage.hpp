#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

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
		enum ShaderType_ {
			ShaderType_2D,
			ShaderType_3D,
			ShaderType_Particles,
			ShaderType_Sky,
			ShaderType_MAX
		};

	public:
		/* MATERIAL */
		struct Material final
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

	public:
		/* MESH */
		struct Mesh final
		{
			enum { Vertices, Indices, Textures };

			Batch<RID, RID, Vector<RID>> data{};
		};

		RID mesh_create();
		void mesh_destroy(RID mesh);

	public:
		/* RENDER TARGET */
		struct RenderTarget final
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
