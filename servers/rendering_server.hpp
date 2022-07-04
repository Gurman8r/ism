#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image.hpp>
#include <servers/rendering/rendering_device.hpp>
#include <servers/rendering/shader_language.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// high-level graphics api
	class ISM_API RenderingServer : public BaseObject
	{
		OBJECT_COMMON(RenderingServer, BaseObject);

		static RenderingServer * g_singleton;

	protected:
		RenderingServer();

	public:
		virtual ~RenderingServer() override;

		FORCE_INLINE static RenderingServer * get_singleton() noexcept { return g_singleton; }

	public:
		virtual void initialize() = 0;
		virtual void finalize() = 0;

	public:
		/* TEXTURE */
		virtual void texture_destroy(RID texture) = 0;
		virtual RID texture_replace(RID old_texture, RID new_texture) = 0;
		virtual RID texture2d_create(Ref<Image> const & image) = 0;
		virtual RID texture2d_placeholder_create() = 0;
		virtual Ref<Image> texture2d_get_image(RID texture) = 0;
		virtual RID texture3d_placeholder_create() = 0;
		virtual RID texturecube_placeholder_create() = 0;

	public:
		/* SHADER */
		virtual RID shader_create() = 0;
		virtual RID shader_placeholder_create() = 0;
		virtual void shader_destroy(RID shader) = 0;

	public:
		/* MATERIAL */
		virtual RID material_create() = 0;
		virtual RID material_placeholder_create() = 0;
		virtual void material_destroy(RID material) = 0;
		virtual RID material_get_shader(RID material) const = 0;
		virtual void material_set_shader(RID material, RID shader) = 0;
		virtual Variant material_get_param(RID material, StringName const & key) const = 0;
		virtual void material_set_param(RID material, StringName const & key, Variant const & value) = 0;
		virtual void material_update(RID material, Map<StringName, Variant> const & params) = 0;

	public:
		/* MESH */
		enum Primitive_
		{
			Primitive_Points,
			Primitive_Lines,
			Primitive_LineStrip,
			Primitive_Triangles,
			Primitive_TriangleStrip,
			Primitive_MAX
		};

		struct SurfaceData
		{
			Primitive_ primitive{};

			DynamicBuffer vertex_data{};
			size_t vertex_count{};

			DynamicBuffer index_data{};
			size_t index_count{};

			RID material{};
		};

		virtual RID mesh_create(Vector<SurfaceData> const & surfaces = {}) = 0;
		virtual RID mesh_placeholder_create() = 0;
		virtual void mesh_destroy(RID mesh) = 0;
		virtual void mesh_clear(RID mesh) = 0;
		virtual void mesh_add_surface(RID mesh, SurfaceData const & surface) = 0;
		virtual size_t mesh_get_surface_count(RID mesh) = 0;
		virtual Primitive_ mesh_surface_get_primitive(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_vertex_array(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_index_array(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_material(RID mesh, size_t index) = 0;

	public:
		/* CAMERA */
		virtual RID camera_create(Vec3 const & position = {}, Vec4 const & rotation = {}) = 0;
		virtual void camera_destroy(RID camera) = 0;
		virtual Vec3 camera_get_position(RID camera) = 0;
		virtual void camera_set_position(RID camera, Vec3 const & value) = 0;
		virtual Vec4 camera_get_rotation(RID camera) = 0;
		virtual void camera_set_rotation(RID camera, Vec4 const & value) = 0;
		virtual Mat4 camera_get_transform(RID camera) = 0;

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
