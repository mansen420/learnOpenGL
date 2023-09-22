#ifndef MESH
#define MESH
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "learn the GL/SHADER.h"
#include "learn the GL/GL_OBJECT.h"

using std::vector;
using std::string;
struct Vertex
{
	glm::vec3 vertexCoords;
	glm::vec3 normalCoords;
	glm::vec2 textureCoords;
};
struct Texture
{
	unsigned int ID;
	std::string type;
};

class Mesh
{
public:
	vector<Vertex> vertices; 
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices; this->indices = indices; this->textures = textures;
	}
	void draw(unsigned int* shaderID)
	{
		setup();				//binds buffer objects

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			glUniform1i(glGetUniformLocation(*shaderID, ("defaultMaterial." + name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
		glActiveTexture(GL_TEXTURE0);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);	//unbinds buffers
	}
private:
	VAO* vao = new VAO(true);		//could cause memory leak
	//set up render data
	void setup()
	{
		vao->bind();
		vao->bindVBO();
		vao->bindEBO();
		vao->vertexBufferData(&vertices, GL_STATIC_DRAW, sizeof(vertices) * sizeof(Vertex));
		vao->elementBufferData(&indices, GL_STATIC_DRAW, sizeof(indices) * sizeof(unsigned int));
		glEnableVertexAttribArray(0);		//vertex coords
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);		//normals 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(2);		//texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}
};

#endif // !MESH

