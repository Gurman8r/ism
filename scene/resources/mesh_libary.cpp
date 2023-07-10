#include <scene/resources/mesh_library.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file.hpp>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	size_t process_vertices(aiMesh const * mesh, DynamicBuffer & data)
	{
		for (size_t i{}; i < (size_t)mesh->mNumVertices; ++i)
		{
			// position
			data << Vec4f{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f };

			// normals
			if (mesh->HasNormals()) {
				data << Vec4f{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.f };
			}
			else {
				data << Vec4f{};
			}

			// texcoords
			if (mesh->HasTextureCoords(0)) {
				data << Vec4f{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z, 1.f };
			}
			else {
				data << Vec4f{};
			}

			// tangents & bitantents
			if (mesh->HasTangentsAndBitangents()) {
				data << Vec4f{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1.f };
				data << Vec4f{ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1.f };
			}
			else {
				data << Vec4f{} << Vec4f{};
			}
		}

		// TODO: properly deduce type
		return data.size() / sizeof(f32);
	}

	size_t process_indices(aiMesh const * mesh, DynamicBuffer & data)
	{
		for (size_t i{}; i < (size_t)mesh->mNumFaces; ++i)
		{
			aiFace face{ mesh->mFaces[i] };

			for (size_t j{}; j < (size_t)face.mNumIndices; ++j)
			{
				data << face.mIndices[j];
			}
		}

		// TODO: properly deduce type
		return data.size() / sizeof(u32);
	}

	void process_aimaterial(aiMaterial const * material, Vector<Ref<Texture>> & textures)
	{
		if (!material) { return; }

		auto _load_material_texture = [&](aiTextureType type, cstring type_name) {
			for (u32 i = 0; i < material->GetTextureCount(type); ++i) {
				aiString path;
				if (material->GetTexture(type, i, &path) != aiReturn_SUCCESS) { continue; }
				textures.push_back(ImageTexture::create(load_resource(path.C_Str())));
			}
		};
		_load_material_texture(aiTextureType_DIFFUSE, "dm"); // diffuse
		_load_material_texture(aiTextureType_AMBIENT, "hm"); // height
		_load_material_texture(aiTextureType_LIGHTMAP, "lm"); // light
		_load_material_texture(aiTextureType_HEIGHT, "nm"); // normal
		_load_material_texture(aiTextureType_SPECULAR, "sm"); // specular
	}

	void process_ainode(aiScene const * scene, aiNode const * node, Vector<RS::SurfaceData> & spec)
	{
		for (size_t i{}; i < (size_t)node->mNumMeshes; ++i)
		{
			aiMesh const * m{ scene->mMeshes[node->mMeshes[i]] };

			RS::SurfaceData & s{ spec.emplace_back(RS::SurfaceData{}) };
			s.primitive = RS::Primitive_Triangles;
			s.vertex_count = process_vertices(m, s.vertex_data);
			s.index_count = process_indices(m, s.index_data);

			//Vector<Ref<Texture>> textures;
			//process_aimaterial(scene->mMaterials[m->mMaterialIndex], textures);
		}

		for (size_t i{}; i < (size_t)node->mNumChildren; ++i)
		{
			process_ainode(scene, node->mChildren[i], spec);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(MeshFormatLoader, t) {}

	Error_ MeshFormatLoader::load_mesh(Ref<Mesh> mesh, String const & path)
	{
		if (!mesh) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }

		if (!File::exists(path)) {
			return Error_Failed;
		}

		if (mesh->m_mesh) { rendering_server()->mesh_destroy(mesh->m_mesh); }

		Assimp::Importer ai;
		aiScene const * scene{ ai.ReadFile(path.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords) };
		ON_SCOPE_EXIT(&ai) { ai.FreeScene(); };

		Vector<RS::SurfaceData> spec;
		process_ainode(scene, scene->mRootNode, spec);
		mesh->m_mesh = rendering_server()->mesh_create(spec);
		if (!mesh->m_mesh) { return Error_Failed; }
		return Error_OK;
	}

	RES MeshFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Mesh> temp{}; temp.instance();
		if (auto const error{ load_mesh(temp, path) }) {
			if (r_error) { *r_error = error; }
			temp = nullptr;
		}
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void MeshFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".obj");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}