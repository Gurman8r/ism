#include <scene/resources/mesh.hpp>
#include <servers/rendering_server.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

EMBED_CLASS(Mesh, t) {}

Mesh::~Mesh()
{
	if (m_array) { SINGLETON(RenderingDevice)->vertexarray_destroy(m_array); }
	
	if (m_indices) { SINGLETON(RenderingDevice)->indexbuffer_destroy(m_indices); }
	
	for (RID vb : m_vertices) { if (vb) { SINGLETON(RenderingDevice)->vertexbuffer_destroy(vb); } }
}

void Mesh::reload_from_file()
{
	if (get_path().empty()) { return; }

	// open scene
	Assimp::Importer _ai;
	aiScene const * scene{ _ai.ReadFile(get_path().c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords) };
	SCOPE_EXIT(&_ai) { _ai.FreeScene(); };

	struct Vertex
	{
		TVec<float_t, 8> data{};

		Vertex(Vec3f const & p, Vec3f const & n, Vec2f const & t) : data{ p[0], p[1], p[2], n[0], n[1], n[2], t[0], t[1] } {}

		auto operator[](size_t i) const { return data[i]; }
	};

	Vector<Vertex> v{};

	Buffer buffer{};

	// for each mesh
	std::for_each(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes], [&](aiMesh * const mesh)
	{
		// for each face
		std::for_each(&mesh->mFaces[0], &mesh->mFaces[mesh->mNumFaces], [&](aiFace const & face)
		{
			// for each index
			std::for_each(&face.mIndices[0], &face.mIndices[face.mNumIndices], [&](uint32_t i)
			{
				if (auto const vp{ mesh->mVertices ? &mesh->mVertices[i] : nullptr }) {
					buffer << vp->x << vp->y << vp->z;
				}
				else {
					buffer << Vec3f{};
				}
				
				if (auto const vn{ mesh->mNormals ? &mesh->mNormals[i] : nullptr }) {
					buffer << vn->x << vn->y << vn->z;
				}
				else {
					buffer << Vec3f{ 1, 1, 1 };
				}
				
				if (auto const uv{ mesh->HasTextureCoords(0) ? &mesh->mTextureCoords[0][i] : nullptr }) {
					buffer << uv->x << uv->y;
				}
				else {
					buffer << Vec2f{ 1, 1 };
				}
			});
		});
	});

	m_array = SINGLETON(RenderingDevice)->vertexarray_create({}, 0, {});
	m_indices = nullptr;
	m_vertices.push_back(SINGLETON(RenderingDevice)->vertexbuffer_create(buffer));
	SINGLETON(RenderingDevice)->vertexarray_update(m_array, VertexLayout{}, m_indices, m_vertices);
}