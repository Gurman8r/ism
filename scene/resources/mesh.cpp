#include <scene/resources/mesh.hpp>
#include <scene/resources/mesh_loader.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Mesh, t) {}

Mesh::Mesh()
{
	m_mesh = RENDERER_STORAGE->mesh_create();
}

Mesh::~Mesh()
{
	if (m_mesh) { RENDERER_STORAGE->mesh_destroy(m_mesh); m_mesh = nullptr; }

	for_each_surface([&](RID vertex_array, RID index_array, Vector<Ref<Texture>> & textures)
	{
		if (index_array) { RENDERING_DEVICE->index_array_destroy(index_array); }

		if (vertex_array) { RENDERING_DEVICE->vertex_array_destroy(vertex_array); }
	});
}

Error_ Mesh::reload_from_file()
{
	if (!get_path()) { return Error_Unknown; }

	return MeshLoader::load_mesh(*this, get_path());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */