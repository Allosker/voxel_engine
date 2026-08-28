#include "assetsManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void AssetsManager::add_model(std::string id, const filepath& path)
{
	std::vector<gfx::Mesh*> loadedMeshes;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		return;
	}

	const auto directory = path.parent_path();

	std::unordered_map<std::string, gfx::Texture*> loadedTextures;

	const auto loadMaterialTextures = [&](gfx::Model& model, aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			gfx::Texture* loadedTexture{};

			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			auto it = loadedTextures.find(str.C_Str());
			if (it != loadedTextures.end())
			{
				loadedTexture = it->second;
			}
			else
			{
				if (str.C_Str()[0] == '*')
				{
					int textureIndex = std::stoi(str.C_Str() + 1);
					aiTexture* embeddedTexture = scene->mTextures[textureIndex];

					if (embeddedTexture->mHeight == 0)
					{
						loadedTexture = &textures.emplace(str.C_Str(), gfx::Texture((u8*)embeddedTexture->pcData, embeddedTexture->mWidth)).first->second;
					}
					else
					{
						assert(embeddedTexture->achFormatHint == std::string("rgba8888") || embeddedTexture->achFormatHint == std::string("argb8888"));
						gfx::Image image(v2u32(embeddedTexture->mWidth, embeddedTexture->mHeight), (u8*)embeddedTexture->pcData, GL_RGBA);
						loadedTexture = &textures.emplace(str.C_Str(), image).first->second;
					}


				}
				else
				{
					loadedTexture = &textures.emplace(str.C_Str(), str.C_Str()).first->second;
				}
			}

			if (loadedTexture)
			{
				loadedTextures[str.C_Str()] = loadedTexture;
				model.textures.push_back(loadedTexture);
			}
		}
	};

	const auto processMesh = [&](aiMesh* mesh, const aiScene* scene)
	{
		std::vector<gfx::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<gfx::Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			gfx::Vertex vertex;
			v3f32 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;

			vertex.pos = vector;

			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				//vertex.Normal = vector;
			}

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				v2f32 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.uvs = vec;
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				//vertex.Tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				//vertex.Bitangent = vector;
			}
			else
				vertex.uvs = {};

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		auto* loadedMesh = &meshes.emplace(id + "/" + mesh->mName.C_Str(), gfx::Mesh(vertices, indices)).first->second;

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		gfx::Model model{loadedMesh};

		loadMaterialTextures(model, material, aiTextureType_DIFFUSE);
		loadMaterialTextures(model, material, aiTextureType_SPECULAR);
		loadMaterialTextures(model, material, aiTextureType_HEIGHT);
		loadMaterialTextures(model, material, aiTextureType_AMBIENT);

		models.emplace(id + "/" + mesh->mName.C_Str(), model);
	};

	const auto processNode = [&](this auto self, aiNode* node, const aiScene* scene) -> void
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			self(node->mChildren[i], scene);
		}

	};

	processNode(scene->mRootNode, scene);
}
