#include <scene/resources/mesh_loader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Mesh, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh(Vector<RS::SurfaceData> const & surfaces)
	{
		if (!surfaces.empty()) { m_mesh = RS::get_singleton()->mesh_create(surfaces); }
	}

	Mesh::~Mesh()
	{
		if (m_mesh) { RS::get_singleton()->mesh_destroy(m_mesh); m_mesh = nullptr; }
	}

	Error_ Mesh::reload_from_file()
	{
		return MeshLoader::load_from_file(*this, get_path());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	size_t Mesh::get_surface_count() const
	{
		return RS::get_singleton()->mesh_get_surface_count(m_mesh);
	}

	RS::Primitive_ Mesh::surface_get_primitive(size_t index) const
	{
		return RS::get_singleton()->mesh_surface_get_primitive(m_mesh, index);
	}

	RID Mesh::surface_get_vertex_array(size_t index) const
	{
		return RS::get_singleton()->mesh_surface_get_vertex_array(m_mesh, index);
	}

	RID Mesh::surface_get_index_array(size_t index) const
	{
		return RS::get_singleton()->mesh_surface_get_index_array(m_mesh, index);
	}

	RID Mesh::surface_get_material(size_t index) const
	{
		return RS::get_singleton()->mesh_surface_get_material(m_mesh, index);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}