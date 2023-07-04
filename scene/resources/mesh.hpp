#ifndef _ISM_MESH_HPP_
#define _ISM_MESH_HPP_

#include <servers/rendering_server.hpp>

namespace Ism
{
	class ISM_API Mesh : public Resource
	{
		DEFINE_CLASS(Mesh, Resource);

		friend class MeshFormatLoader;

		RID m_mesh{};

	public:
		Mesh() noexcept = default;
		Mesh(Vector<RS::SurfaceData> const & surfaces);
		Mesh(RS::SurfaceData const & surface) : Mesh{ Vector<RS::SurfaceData>{ surface } } {}
		virtual ~Mesh() override;
		virtual RID get_rid() const override { return m_mesh; }

	public:
		virtual size_t get_surface_count() const;
		virtual RS::Primitive_ surface_get_primitive(size_t index) const;
		virtual RID surface_get_vertex_array(size_t index) const;
		virtual RID surface_get_index_array(size_t index) const;
		virtual RID surface_get_material(size_t index) const;

	public:
		static void draw(Ref<Mesh> const & mesh);
	};
}

#endif // !_ISM_MESH_HPP_
