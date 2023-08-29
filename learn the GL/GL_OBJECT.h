#ifndef VAO_H
#define VAO_H
#include <glad/glad.h>
#include <iostream>
class GLObject
{
public:
	unsigned int* ID = nullptr;

	virtual void bind() const = 0;
	virtual bool isBound() const = 0;

	virtual void kill() = 0;
	bool isDead() const
	{
		return ID == nullptr;
	}
	virtual void generate(unsigned int& objectID) = 0;
};
class dataObject : public GLObject
{
public:
	dataObject(){}
	dataObject(unsigned int& objectID)
	{
		glGenBuffers(1, &objectID);
		ID = &objectID;
	}
	virtual void bufferData(const void* data, GLenum usage) const = 0;
	void kill() override
	{
		if(isDead())
			std::cout << "GL_OJBECT::dataObject::kill::this object is already dead" << std::endl;
		else
		{
			glDeleteBuffers(1, ID);
			ID = nullptr;
		}
	}
	void generate(unsigned int& objectID) 
	{
		if (isDead())
		{
			glGenBuffers(1, &objectID);
			ID = &objectID;
		}
		else
		{
			std::cout << "GL_OBJECT::dataObject::generate::this object is already generated" << std::endl;
		}
	}
};

class VBO : public dataObject
{
public:
	VBO(){}
	VBO(unsigned int& objectID) : dataObject(objectID)
	{
	}
	void bind() const override
	{
		if (isDead())
			std::cout << "GL_OBJECT::VBO::bind::this object is dead" << std::endl;
		else
			glBindBuffer(GL_ARRAY_BUFFER, *ID);
	}
	void bufferData(const void* data, GLenum usage) const override
	{
		if (isBound())
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, usage);
		else
			std::cout << "GL_OJBECT::VBO::bufferData::this VBO is not currently bound" << std::endl;
	}
	bool isBound() const override
	{
		int data;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &data);
		return data == *ID;
	}
};
class EBO : public dataObject
{
public:
	EBO(){}
	EBO(unsigned int& objectID) : dataObject(objectID)
	{
	}
	void bind() const override
	{
		if (isDead())
			std::cout << "GL_OBJECT::EBO::bind::this object is dead" << std::endl;
		else
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ID);
	}
	void bufferData(const void* data, GLenum usage) const override
	{
		if (isBound())
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, usage);
		else
			std::cout << "GL_OJBECT::EBO::bufferData::this EBO is not currently bound" << std::endl;
	}
	bool isBound() const override
	{
		int data;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &data);
		return data == *ID;
	}
};
class VAO : public GLObject
{
private:
	VBO* vertexBufferObjectPtr = new VBO();		//for pre-exsting objects
	EBO* elementBufferObjectPtr = new EBO();
	VBO vertexBufferObject;						//embedded objects
	EBO elementBufferObject;
public:
	//TODO add overloading for pre-exsting objects
	VAO(unsigned int& VAOobjectID)
	{
		glGenVertexArrays(1, &VAOobjectID);
		ID = &VAOobjectID;
	}
	VAO(unsigned int& VAOobjectID, unsigned int& VBOobjectID)
	{
		glGenVertexArrays(1, &VAOobjectID);
		ID = &VAOobjectID;
		vertexBufferObject = VBO(VAOobjectID);
	}
	VAO(unsigned int& VAOobjectID, unsigned int& VBOobjectID, unsigned int& EBOobjectID)
	{
		glGenVertexArrays(1, &VAOobjectID);
		ID = &VAOobjectID;
		vertexBufferObject = VBO(VAOobjectID);
		elementBufferObject = EBO(EBOobjectID);
	}

	bool isBound() const override
	{
		int data;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &data);
		return data == *ID;
	}
	bool haveVBO() const
	{
		return (!vertexBufferObjectPtr->isDead())||(!vertexBufferObject.isDead());
	}
	bool haveEBO() const
	{
		return (!elementBufferObjectPtr->isDead()) || (!elementBufferObject.isDead());
	}

	void bind() const override
	{
		if (isDead())
			std::cout << "GL_OBJECT::VAO::bind::this object is dead" << std::endl;
		else
			glBindVertexArray(*ID);
	}

	void addVBO(unsigned int& VBOobjectID)
	{
		if (haveVBO())
			std::cout << "GL_OBJECT::VAO::addVBO::this VAO already has an associated VBO" << std::endl;
		else
		{
			vertexBufferObject = VBO(VBOobjectID);
		}
	}
	void addVBO(VBO* vertexBufferObjectPTR)
	{
		if (haveVBO())
			std::cout << "GL_OBJECT::VAO::addVBO::this VAO already has an associated VBO" << std::endl;
		else
		{
			delete vertexBufferObjectPtr;
			vertexBufferObjectPtr = vertexBufferObjectPTR;
		}
	}
	void bindVBO() const
	{
		if (haveVBO())
		{
			if (isBound())
			{
				if (!vertexBufferObjectPtr->isDead())
					vertexBufferObjectPtr->bind();
				if (!vertexBufferObject.isDead())
					vertexBufferObject.bind();
			}
			else
				std::cout << "GL_OJBECT::VAO::bindVBO::this VAO is not currently bound" << std::endl;
		}
		else
			std::cout << "GL_OBJECT::VAO::bindVBO::no VBO objectID provided" << std::endl;
	}
	void overwriteVBO(unsigned int& VBOojectID)
	{
		if (haveVBO())
		{
			if (!vertexBufferObject.isDead())
				vertexBufferObject.kill();
			if (!vertexBufferObjectPtr->isDead())
				vertexBufferObjectPtr->kill();
			vertexBufferObject = VBO(VBOojectID);
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteVBO::this VAO does not have a VBO to overwrite" << std::endl;
		}
	}
	void overwriteVBO(VBO* vertexBufferObjectPTR)
	{
		if (haveVBO())
		{
			if (!vertexBufferObject.isDead())
				vertexBufferObject.kill();
			if (!vertexBufferObjectPtr->isDead())
				vertexBufferObjectPtr->kill();
			vertexBufferObjectPtr = vertexBufferObjectPTR;
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteVBO::this VAO does not have a VBO to overwrite" << std::endl;
		}
	}
	void vertexBufferData(const void* data, GLenum usage) const
	{
		if (isBound())
		{
			if (haveVBO())
			{
				if (vertexBufferObjectPtr->isBound())
					vertexBufferObjectPtr->bufferData(data, usage);
				if (vertexBufferObject.isBound())
					vertexBufferObject.bufferData(data, usage);
				else
					std::cout << "GL_OBJECT::VAO::vertexBufferData::this VBO is not currently bound" << std::endl;
			}
			else
				std::cout << "GL_OBJECT::VAO::vertexBufferData::no VBO objectID provided" << std::endl;
		}
		else
			std::cout << "GL_OJBECT::VAO::vertexBufferData::this VAO is not currently bound" << std::endl;
	}

	void addEBO(unsigned int& EBOobjectID)
	{
		if (haveEBO())
			std::cout << "GL_OBJECT::VAO::addEBO::this VAO already has an associated EBO" << std::endl;
		else
		{
			elementBufferObject = EBO(EBOobjectID);
		}

	}
	void addEBO(EBO* elementBufferObjectPTR)
	{
		if (haveEBO())
			std::cout << "GL_OBJECT::VAO::addEBO::this VAO already has an associated EBO" << std::endl;
		else
		{
			delete elementBufferObjectPtr;
			elementBufferObjectPtr = elementBufferObjectPTR;
		}

	}
	void bindEBO() const
	{
		if (haveEBO())
		{
			if (isBound())
			{
				if (!elementBufferObject.isDead())
					elementBufferObject.bind();
				if (!elementBufferObjectPtr->isDead())
					elementBufferObjectPtr->bind();
			}
			else
				std::cout << "GL_OJBECT::VAO::bindEBO::this VAO is not currently bound" << std::endl;
		}
		else
			std::cout << "GL_OBJECT::VAO::bindEBO::no EBO objectID provided" << std::endl;
	}
	void overwriteEBO(unsigned int& EBOojectID)
	{
		if (haveEBO())
		{
			if (!elementBufferObject.isDead())
				elementBufferObject.kill();
			if (!elementBufferObjectPtr->isDead())
				elementBufferObjectPtr->kill();
			elementBufferObject = EBO(EBOojectID);
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteEBO::this VAO does not have an EBO to overwrite" << std::endl;
		}
	}
	void overwriteEBO(EBO* elementBufferObjectPTR)
	{
		if (haveEBO())
		{
			if (!elementBufferObject.isDead())
				elementBufferObject.kill();
			if (!elementBufferObjectPtr->isDead())
				elementBufferObjectPtr->kill();
			elementBufferObjectPtr = elementBufferObjectPTR;
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteEBO::this VAO does not have an EBO to overwrite" << std::endl;
		}
	}
	void elementBufferData(const void* data, GLenum usage) const
	{
		if (isBound())
		{
			if (haveEBO())
			{
				if (elementBufferObjectPtr->isBound())
					elementBufferObjectPtr->bufferData(data, usage);
				if (elementBufferObject.isBound())
					elementBufferObject.bufferData(data, usage);
				else
					std::cout << "GL_OBJECT::VAO::elementBufferData::this EBO is not currently bound" << std::endl;
			}
			else
				std::cout << "GL_OBJECT::VAO::elementBufferData::no EBO objectID provided" << std::endl;
		}
		else
			std::cout << "GL_OJBECT::VAO::elementBufferData::this VAO is not currently bound" << std::endl;
	}

	void kill() override
	{
		if (isDead())
			std::cout << "GL_OJBECT::VAO::kill::this VAO is already dead" << std::endl;
		else
		{
			glDeleteVertexArrays(1, ID);
			ID = nullptr;
		}
	}
	void generate(unsigned int& objectID) override
	{
		if (isDead())
		{
			glGenVertexArrays(1, &objectID);
			ID = &objectID; 
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::generate::this object is already generated" << std::endl;
		}
	}

};

#endif // !VAO

