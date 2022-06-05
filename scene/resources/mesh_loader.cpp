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

size_t _load_vertices(aiMesh const * mesh, DynamicBuffer & data)
{
	ASSERT(mesh);

	for (size_t i = 0; i < (size_t)mesh->mNumVertices; ++i)
	{
		// position
		data << Vec4{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f };

		// normals
		if (mesh->HasNormals())
		{
			data << Vec4{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.f };
		}
		else
		{
			data << Vec4{ 0, 0, 0, 1 };
		}

		// texcoords
		if (mesh->HasTextureCoords(0))
		{
			data << Vec4{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z, 1.f };
		}
		else
		{
			data << Vec4{ 0, 0, 0, 1 };
		}

		// tangents & bitantents
		if (mesh->HasTangentsAndBitangents())
		{
			data << Vec4{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1.f };
			data << Vec4{ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1.f };
		}
		else
		{
			data << Vec4{ 0, 0, 0, 1 } << Vec4{ 0, 0, 0, 1 };
		}

		// blending
		data << Vec4i{ 1, 1, 1, 1 };
	}

	return data.size() / get_data_type_size(DataType_F32);
}

size_t _load_indices(aiMesh const * mesh, DynamicBuffer & data)
{
	ASSERT(mesh);

	for (size_t i = 0; i < (size_t)mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (size_t j = 0; j < (size_t)face.mNumIndices; ++j)
		{
			data << face.mIndices[j];
		}
	}

	return data.size() / RD::get_index_buffer_format_size(RD::IndexbufferFormat_U32);
}

void _load_textures(aiMaterial const * material, Vector<Ref<Texture>> & textures)
{
	if (!material) { return; }

	auto _load_material_texture = [&](aiTextureType type, cstring type_name)
	{
		for (uint32_t i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString path; material->GetTexture(type, i, &path);
			
			Ref<ImageTexture> texture{ Path{ path.C_Str() } };
			
			textures.push_back(texture);
		}
	};
	_load_material_texture(aiTextureType_DIFFUSE, "dm"); // diffuse
	_load_material_texture(aiTextureType_AMBIENT, "hm"); // height
	_load_material_texture(aiTextureType_LIGHTMAP, "lm"); // light
	_load_material_texture(aiTextureType_HEIGHT, "nm"); // normal
	_load_material_texture(aiTextureType_SPECULAR, "sm"); // specular
}

void _process_node(aiScene const * scene, aiNode const * node, Vector<RS::SurfaceData> & surfaces)
{
	for (size_t i = 0; i < (size_t)node->mNumMeshes; ++i)
	{
		aiMesh const * m{ scene->mMeshes[node->mMeshes[i]] };

		RS::SurfaceData & s{ surfaces.emplace_back(RS::SurfaceData{}) };
		s.primitive = RD::PrimitiveType_Triangles;
		s.vertex_count = _load_vertices(m, s.vertex_data);
		s.index_count = _load_indices(m, s.index_data);

		Vector<Ref<Texture>> textures;
		_load_textures(scene->mMaterials[m->mMaterialIndex], textures);
	}

	for (size_t i = 0; i < (size_t)node->mNumChildren; ++i)
	{
		_process_node(scene, node->mChildren[i], surfaces);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ MeshLoader::load_mesh(Mesh & mesh, Path const & path)
{
	if (!path) { return Error_Unknown; }

	Assimp::Importer ai;
	aiScene const * scene{ ai.ReadFile(path.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords) };
	SCOPE_EXIT(&ai) { ai.FreeScene(); };

	Vector<RS::SurfaceData> surfaces;
	_process_node(scene, scene->mRootNode, surfaces);
	if (mesh.m_mesh) { RENDERING_SERVER->mesh_destroy(mesh.m_mesh); }
	mesh.m_mesh = RENDERING_SERVER->mesh_create(surfaces);

	return Error_None;
}

Error_ MeshLoader::load_mesh(Ref<Mesh> mesh, Path const & path)
{
	if (!mesh) { return Error_Unknown; }

	return load_mesh(**mesh, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */