#include <scene/resources/mesh_loader.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>

using namespace ism;

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static constexpr RD::VertexLayoutAttribute const default_mesh_format[6]{
{ "attr_position"	, DataType_F32, 4 },
{ "attr_normal"		, DataType_F32, 4 },
{ "attr_texcoord"	, DataType_F32, 4 },
{ "attr_tangent"	, DataType_F32, 4 },
{ "attr_bitangent"	, DataType_F32, 4 },
{ "attr_blending"	, DataType_I32, 4 },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static DynamicBuffer _process_vertices(aiMesh * mesh)
{
	ASSERT(mesh);
	DynamicBuffer vertex_data{};
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
	{
		// position
		vertex_data << Vec4{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f };

		// normals
		if (mesh->HasNormals()) {
			vertex_data << Vec4{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.f };
		}
		else {
			vertex_data << Vec4{ 0, 0, 0, 1 };
		}

		// texcoords
		if (mesh->HasTextureCoords(0)) {
			vertex_data << Vec4{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z, 1.f };
		}
		else {
			vertex_data << Vec4{ 0, 0, 0, 1 };
		}

		// tangents & bitantents
		if (mesh->HasTangentsAndBitangents()) {
			vertex_data << Vec4{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1.f };
			vertex_data << Vec4{ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1.f };
		}
		else {
			vertex_data << Vec4{ 0, 0, 0, 1 } << Vec4{ 0, 0, 0, 1 };
		}

		// blending
		vertex_data << Vec4i{ 1, 1, 1, 1 };
	}
	return vertex_data;
}

static DynamicBuffer _process_indices(aiMesh * mesh)
{
	ASSERT(mesh);
	DynamicBuffer index_data{};
	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			index_data << face.mIndices[j];
		}
	}
	return index_data;
}

static void _process_material(aiMaterial * material, Vector<Ref<Texture>> & textures)
{
	if (!material) { return; }

	// textures
	static auto _load_material_textures = [material, &textures](aiTextureType type, cstring type_name) {
		for (uint32_t i = 0; i < material->GetTextureCount(type); ++i) {
			aiString str; material->GetTexture(type, i, &str);
			Ref<ImageTexture> tex; tex.instance(str.C_Str());
			textures.push_back(tex);
		}
	};
	_load_material_textures(aiTextureType_DIFFUSE, "dm"); // diffuse
	_load_material_textures(aiTextureType_AMBIENT, "hm"); // height
	_load_material_textures(aiTextureType_LIGHTMAP, "lm"); // light
	_load_material_textures(aiTextureType_HEIGHT, "nm"); // normal
	_load_material_textures(aiTextureType_SPECULAR, "sm"); // specular
}

static void _process_mesh(Mesh::SurfaceData & surfaces, aiMesh * mesh, aiScene const * scene)
{
	// process vertices
	DynamicBuffer vertex_data{ _process_vertices(mesh) };
	DataType_ const vertex_type{ DataType_F32 };
	size_t const vertex_size{ get_data_type_size(vertex_type) };
	size_t const vertex_count{ vertex_data.size() / vertex_size };
	RID vb{ RENDERING_DEVICE->vertex_buffer_create(vertex_data.size(), std::move(vertex_data)) };
	RID va{ RENDERING_DEVICE->vertex_array_create(vertex_count, default_mesh_format, { vb }) };

	// process indices
	DynamicBuffer index_data{ _process_indices(mesh) };
	RD::IndexbufferFormat_ const index_type{ RD::IndexbufferFormat_U32 };
	size_t const index_size{ RD::get_index_buffer_format_size(index_type) };
	size_t const index_count{ index_data.size() / index_size };
	RID ib{ RENDERING_DEVICE->index_buffer_create(index_count, index_type, std::move(index_data)) };
	RID ia{ RENDERING_DEVICE->index_array_create(ib, 0, index_count) };

	// process materials
	Vector<Ref<Texture>> textures{};
	_process_material(scene->mMaterials[mesh->mMaterialIndex], textures);

	// done
	surfaces.push_back(va, ia, std::move(textures));
}

static void _process_node(Mesh::SurfaceData & surfaces, aiNode * node, aiScene const * scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
		_process_mesh(surfaces, scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i) {
		_process_node(surfaces, node->mChildren[i], scene);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ MeshLoader::load_mesh(Mesh & mesh, Path const & path)
{
	if (path.empty()) { return Error_Unknown; }

	Assimp::Importer ai;
	aiScene const * scene{ ai.ReadFile(path.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords) };
	SCOPE_EXIT(&ai) { ai.FreeScene(); };

	_process_node(mesh.m_surface_data, scene->mRootNode, scene);

	return Error_None;
}

Error_ MeshLoader::load_mesh(Ref<Mesh> mesh, Path const & path)
{
	if (!mesh) { return Error_Unknown; }

	return load_mesh(**mesh, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */