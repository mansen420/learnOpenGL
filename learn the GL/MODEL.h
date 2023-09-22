#ifndef MODEL
#define MODEL
#include "MESH.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

            }
        }

    }
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif // !MODEL

