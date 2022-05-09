#include <scene/resources/mesh.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

struct SubMesh
{
	RD::VertexFormat format{};

	DynamicBuffer vertices{};

	DynamicBuffer indices{};

	Vector<Ref<Texture>> textures{};
};

static void _load_material_textures(Vector<Ref<Texture>> & textures, aiMaterial * mat, aiTextureType type, cstring type_name)
{
	for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		Ref<ImageTexture> tex;
		tex.instance(str.C_Str());

		textures.push_back(tex);
	}
}

static SubMesh _process_mesh(aiMesh * mesh, aiScene const * scene)
{
	// format
	RD::VertexFormat format{
		{ "a_Position"	, DataType_F32, 3 },
		{ "a_Normal"	, DataType_F32, 3 },
		{ "a_Texcoord"	, DataType_F32, 2 },
		{ "a_Tangent"	, DataType_F32, 3 },
		{ "a_Bitangent"	, DataType_F32, 3 }, };

	// vertices
	DynamicBuffer vertices{};
	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices << mesh->mVertices[i].x << mesh->mVertices[i].y << mesh->mVertices[i].z;

		if (mesh->HasNormals()) {
			vertices << mesh->mNormals[i].x << mesh->mNormals[i].y << mesh->mNormals[i].z;
		}
		else {
			vertices << Vec3f{};
		}

		if (mesh->HasTextureCoords(0)) {
			vertices << mesh->mTextureCoords[0][i].x << mesh->mTextureCoords[0][i].y;
		}
		else {
			vertices << Vec2f{};
		}

		if (mesh->HasTangentsAndBitangents()) {
			vertices << mesh->mTangents[i].x << mesh->mTangents[i].y << mesh->mTangents[i].z;
			vertices << mesh->mBitangents[i].x << mesh->mBitangents[i].y << mesh->mBitangents[i].z;
		}
		else {
			vertices << Vec3f{} << Vec3f{};
		}
	}

	// indices
	DynamicBuffer indices{};
	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			indices << face.mIndices[j];
		}
	}

	// textures
	Vector<Ref<Texture>> textures{};
	aiMaterial * material{ scene->mMaterials[mesh->mMaterialIndex] };
	_load_material_textures(textures, material, aiTextureType_DIFFUSE, "texture_diffuse");
	_load_material_textures(textures, material, aiTextureType_SPECULAR, "texture_specular");
	_load_material_textures(textures, material, aiTextureType_HEIGHT, "texture_normal");
	_load_material_textures(textures, material, aiTextureType_AMBIENT, "texture_height");

	// create submesh
	return SubMesh{ std::move(format), std::move(vertices), std::move(indices), std::move(textures) };
}

static void _process_node(Vector<SubMesh> & meshes, aiNode * node, aiScene const * scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
	{
		meshes.push_back(_process_mesh(scene->mMeshes[node->mMeshes[i]], scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		_process_node(meshes, node->mChildren[i], scene);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Mesh, t) {}

Mesh::~Mesh()
{
	each([&](RID vertex_array, RID index_array, Vector<Ref<Texture>> & textures)
	{
		if (index_array) { RENDERING_DEVICE->index_array_destroy(index_array); }

		if (vertex_array) { RENDERING_DEVICE->vertex_array_destroy(vertex_array); }
	});
}

void Mesh::reload_from_file()
{
	if (get_path().empty()) { return; }

	Assimp::Importer _ai;
	aiScene const * scene{ _ai.ReadFile(get_path().c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords) };
	SCOPE_EXIT(&_ai) { _ai.FreeScene(); };

	Vector<SubMesh> meshes{};
	_process_node(meshes, scene->mRootNode, scene);
	for (SubMesh & m : meshes)
	{
		RD::IndexbufferFormat_ const index_type{ RD::IndexbufferFormat_U32 };
		size_t const index_size{ RD::get_index_buffer_format_size(index_type) };
		size_t const index_count{ m.indices.size() / index_size };
		RID ib{ RENDERING_DEVICE->index_buffer_create(index_count, index_type, m.indices) };
		RID ia{ RENDERING_DEVICE->index_array_create(ib, 0, index_count) };

		DataType_ const vertex_type{ DataType_F32 };
		size_t const vertex_size{ get_data_type_size(vertex_type) };
		size_t const vertex_count{ m.vertices.size() / vertex_size };
		RID vb{ RENDERING_DEVICE->vertex_buffer_create(m.vertices.size(), m.vertices) };
		RID va{ RENDERING_DEVICE->vertex_array_create(vertex_count, m.format, { vb }) };

		m_data.push_back(va, ia, std::move(m.textures));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */