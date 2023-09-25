#ifndef MODEL
#define MODEL
#include "MESH.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void draw(Shader& shader)
    {
        for (int i = 0; i < meshes.size(); i++)
            meshes[i].draw(&shader.ID);
    }
private:
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (int i =0;i<node->mNumMeshes;i++)
        {
            aiMesh* temp_mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(temp_mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex temp_vertex;

            temp_vertex.vertexCoords.x = mesh->mVertices[i].x;
            temp_vertex.vertexCoords.y = mesh->mVertices[i].y;
            temp_vertex.vertexCoords.z = mesh->mVertices[i].z;

            temp_vertex.normalCoords.x = mesh->mNormals[i].x;
            temp_vertex.normalCoords.y = mesh->mNormals[i].y;
            temp_vertex.normalCoords.z = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0])        //if there is a texture
            {
                temp_vertex.textureCoords.x = mesh->mTextureCoords[0][i].x;
                temp_vertex.textureCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else
                temp_vertex.textureCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(temp_vertex);
        }
        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        if (mesh->mMaterialIndex>=0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        return Mesh(vertices, indices, textures);
    }
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture temp_tex;
        //    temp_tex.ID = TextureFromFile(str.C_Str(), directory);
            temp_tex.path = str.C_Str();
            temp_tex.type = typeName;
            textures.push_back(temp_tex);
        }
        return textures;
    }
    unsigned int TextureFromFile(std::string path, std::string directory)
    {
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

#endif // !MODEL

