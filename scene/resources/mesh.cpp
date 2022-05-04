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
	VertexFormat layout{};

	Buffer vertices{}, indices{};

	Vector<Ref<Texture>> textures{};
};

static void _load_material_textures(Vector<Ref<Texture>> & textures, aiMaterial * mat, aiTextureType type, cstring type_name)
{
	for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// TODO: load the texture...
	}
}

static SubMesh _process_mesh(aiMesh * mesh, aiScene const * scene)
{
	VertexFormat layout{};

	Buffer vertices{}, indices{};

	for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices << mesh->mVertices[i].x << mesh->mVertices[i].y << mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertices << mesh->mNormals[i].x << mesh->mNormals[i].y << mesh->mNormals[i].z;
		}
		else
		{
			vertices << Vec3f{};
		}

		if (mesh->HasTextureCoords(0))
		{
			vertices << mesh->mTextureCoords[0][i].x << mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertices << Vec2f{};
		}

		//if (mesh->HasTangentsAndBitangents())
		//{
		//	vertices << mesh->mTangents[i].x << mesh->mTangents[i].y << mesh->mTangents[i].z;
		//	vertices << mesh->mBitangents[i].x << mesh->mBitangents[i].y << mesh->mBitangents[i].z;
		//}
		//else
		//{
		//	vertices << Vec3f{} << Vec3f{};
		//}
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			indices << face.mIndices[j];
		}
	}

	Vector<Ref<Texture>> textures{};
	aiMaterial * material{ scene->mMaterials[mesh->mMaterialIndex] };
	_load_material_textures(textures, material, aiTextureType_DIFFUSE, "texture_diffuse");
	_load_material_textures(textures, material, aiTextureType_SPECULAR, "texture_specular");
	_load_material_textures(textures, material, aiTextureType_HEIGHT, "texture_normal");
	_load_material_textures(textures, material, aiTextureType_AMBIENT, "texture_height");

	return SubMesh{ std::move(layout), std::move(vertices), std::move(indices), std::move(textures) };
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
	for_data([](RID vertexarray, RID indexbuffer, auto &)
	{
		if (indexbuffer) { RENDERING_DEVICE->indexbuffer_destroy(indexbuffer); }
		if (vertexarray) { RENDERING_DEVICE->vertexarray_destroy(vertexarray); }
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
	for (SubMesh & submesh : meshes)
	{
		RID const ib{ RENDERING_DEVICE->indexbuffer_create(submesh.indices) };
		RID const vb{ RENDERING_DEVICE->vertexbuffer_create(submesh.vertices) };
		RID const va{ RENDERING_DEVICE->vertexarray_create(submesh.layout, { vb }) };
		m_data.push_back(va, ib, std::move(submesh.textures));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */