#include <scene/resources/mesh.hpp>
#include <scene/resources/mesh_loader.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Mesh, t) {}

Mesh::~Mesh()
{
	if (m_mesh) { RENDERING_SERVER->mesh_destroy(m_mesh); m_mesh = nullptr; }
}

Error_ Mesh::reload_from_file()
{
	return MeshLoader::load_mesh(*this, get_path());
}

size_t Mesh::get_surface_count() const
{
	return RENDERING_SERVER->mesh_get_surface_count(m_mesh);
}

RID Mesh::get_vertex_array(size_t index) const
{
	return RENDERING_SERVER->mesh_surface_get_vertex_array(m_mesh, index);
}

RID Mesh::get_index_array(size_t index) const
{
	return RENDERING_SERVER->mesh_surface_get_index_array(m_mesh, index);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */