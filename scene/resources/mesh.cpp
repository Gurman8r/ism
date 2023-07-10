#include <scene/resources/mesh_library.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Mesh, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh(Vector<RS::SurfaceData> const & surfaces)
	{
		if (!surfaces.empty()) { m_mesh = rendering_server()->mesh_create(surfaces); }
	}

	Mesh::~Mesh()
	{
		if (m_mesh) { rendering_server()->mesh_destroy(m_mesh); m_mesh = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	size_t Mesh::get_surface_count() const
	{
		return rendering_server()->mesh_get_surface_count(m_mesh);
	}

	RS::Primitive_ Mesh::surface_get_primitive(size_t index) const
	{
		return rendering_server()->mesh_surface_get_primitive(m_mesh, index);
	}

	RID Mesh::surface_get_vertex_array(size_t index) const
	{
		return rendering_server()->mesh_surface_get_vertex_array(m_mesh, index);
	}

	RID Mesh::surface_get_index_array(size_t index) const
	{
		return rendering_server()->mesh_surface_get_index_array(m_mesh, index);
	}

	RID Mesh::surface_get_material(size_t index) const
	{
		return rendering_server()->mesh_surface_get_material(m_mesh, index);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::draw(Ref<Mesh> const & mesh)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}