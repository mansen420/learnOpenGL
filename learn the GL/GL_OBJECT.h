#ifndef VAO_H
#define VAO_H
#include <glad/glad.h>
#include <iostream>
class GLObject
{
protected:
	unsigned int ID = 0;
public:

	virtual void bind() const = 0;
	virtual bool isBound() const = 0;

	virtual void kill() = 0;
	virtual void generate() = 0;

	bool isDead() const { return ID == 0; }
	const unsigned int& getID() { return ID; }
};
class dataObject : public GLObject
{
public:
	dataObject(bool generateObjects = true)
	{
		if (generateObjects)
		{
			glGenBuffers(1, &ID);
		}
	}
	//beware that sizeof(data) wouldn't work, as it would only return the size of the pointer, not the actual array 
	virtual void bufferData(const void* data, GLenum usage, GLsizeiptr size) const = 0; 
	void kill() override
	{
		if(isDead())
			std::cout << "GL_OJBECT::dataObject::kill::this object is already dead" << std::endl;
		else
		{
			glDeleteBuffers(1, &ID);
			ID = 0;
		}
	}
	void generate() override
	{
		if (isDead())
		{
			glGenBuffers(1, &ID);
		}
		else
		{
			std::cout << "GL_OBJECT::dataObject::generate::this object is already generated" << std::endl;
		}
	}
};

//TODO copy initialization, or = operator, should either be forbidden, or they should cause the dangling object to be killed.
class VBO : public dataObject
{
public:
	VBO(bool generateObjects = true) : dataObject(generateObjects)
	{
	}
	void bind() const override
	{
		if (isDead())
			std::cout << "GL_OBJECT::VBO::bind::this object is dead" << std::endl;
		else
			glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	void bufferData(const void* data, GLenum usage, GLsizeiptr size ) const override	
	{
		if (isBound())
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		else
			std::cout << "GL_OJBECT::VBO::bufferData::this VBO is not currently bound" << std::endl;
	}
	bool isBound() const override
	{
		if (isDead())
			return false;
		int data = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &data);
		return data == ID;
	}
	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;
	~VBO() { kill(); }
};
class EBO : public dataObject
{
public:
	EBO(bool generateObjects = true) : dataObject(generateObjects)
	{
	}
	void bind() const override
	{
		if (isDead())
			std::cout << "GL_OBJECT::EBO::bind::this object is dead" << std::endl;
		else
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}
	void bufferData(const void* data, GLenum usage, GLsizeiptr size) const override
	{
		if (isBound())
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		else
			std::cout << "GL_OJBECT::EBO::bufferData::this EBO is not currently bound" << std::endl;
	}
	bool isBound() const override
	{
		if (isDead())
			return false;
		int data = 0;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &data);
		return data == ID;
	}
	EBO(const EBO&) = delete;
	EBO& operator=(const EBO&) = delete;
	~EBO() { kill(); }
};
class VAO : public GLObject
{
public:	//temporary solution 
	VBO* vertexBufferObjectPtr = new VBO(false);		//for pre-exsting objects
	EBO* elementBufferObjectPtr = new EBO(false);
	VBO vertexBufferObject;				//embedded objects
	EBO elementBufferObject;
public:
	//TODO add overloading for pre-exsting objects
	VAO()
	{	
		//I dont know why the compiler won't let me initialize these objects with false so I am killing them here.
		vertexBufferObject.kill();
		elementBufferObject.kill();
		glGenVertexArrays(1, &ID);
	}
	VAO(bool generateBuffers)
	{
		if (generateBuffers)
		{
			glGenVertexArrays(1, &ID);
		}
	}

	bool isBound() const override
	{
		int data = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &data);
		return data == ID;
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
			glBindVertexArray(ID);
	}

	void addVBO()
	{
		if (haveVBO())
			std::cout << "GL_OBJECT::VAO::addVBO::this VAO already has an associated VBO" << std::endl;
		else
		{
			vertexBufferObject.generate();
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
	void overwriteVBO()
	{
		if (haveVBO())
		{
			if (!vertexBufferObject.isDead())
				vertexBufferObject.kill();
			if (!vertexBufferObjectPtr->isDead())
				vertexBufferObjectPtr = new VBO(false);
			vertexBufferObject.generate();
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteVBO::this VAO does not have a VBO to overwrite" << std::endl;
		}
	}
	void overwriteVBO(VBO* vertexBufferObjectPTR)		//beware that this function will call kill() on any correspondig buffer object assigned to this VAO
	{
		if (haveVBO())
		{
			if (!vertexBufferObject.isDead())
				vertexBufferObject.kill();
			if (vertexBufferObjectPtr->isDead())
				delete vertexBufferObjectPtr;
			vertexBufferObjectPtr = vertexBufferObjectPTR;
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteVBO::this VAO does not have a VBO to overwrite" << std::endl;
		}
	}
	void vertexBufferData(const void* data, GLenum usage, GLsizeiptr size) const
	{
		if (isBound())
		{
			if (haveVBO())
			{
				if (vertexBufferObjectPtr->isBound())
					vertexBufferObjectPtr->bufferData(data, usage, size);
				else if (vertexBufferObject.isBound())
					vertexBufferObject.bufferData(data, usage, size);
				else
					std::cout << "GL_OBJECT::VAO::vertexBufferData::this VBO is not currently bound" << std::endl;
			}
			else
				std::cout << "GL_OBJECT::VAO::vertexBufferData::no VBO objectID provided" << std::endl;
		}
		else
			std::cout << "GL_OJBECT::VAO::vertexBufferData::this VAO is not currently bound" << std::endl;
	}

	void addEBO()
	{
		if (haveEBO())
			std::cout << "GL_OBJECT::VAO::addEBO::this VAO already has an associated EBO" << std::endl;
		else
		{
			elementBufferObject.generate();
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
	void overwriteEBO()
	{
		if (haveEBO())
		{
			if (!elementBufferObject.isDead())
				elementBufferObject.kill();
			if (!elementBufferObjectPtr->isDead())
				elementBufferObjectPtr = new EBO(false);
			elementBufferObject.generate();
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
			if (elementBufferObjectPtr->isDead())
				delete elementBufferObjectPtr;
			elementBufferObjectPtr = elementBufferObjectPTR;
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::overwriteEBO::this VAO does not have an EBO to overwrite" << std::endl;
		}
	}
	void elementBufferData(const void* data, GLenum usage, GLsizeiptr size) const
	{
		if (isBound())
		{
			if (haveEBO())
			{
				if (elementBufferObjectPtr->isBound())
					elementBufferObjectPtr->bufferData(data, usage, size);
				else if (elementBufferObject.isBound())
					elementBufferObject.bufferData(data, usage, size);
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
			if (vertexBufferObjectPtr->isDead())
				delete vertexBufferObjectPtr;
			if (elementBufferObjectPtr->isDead())
				delete elementBufferObjectPtr;
			glDeleteVertexArrays(1, &ID);
			ID = 0;
		}
	}
	void generate() override
	{
		if (isDead())
		{
			glGenVertexArrays(1, &ID);
		}
		else
		{
			std::cout << "GL_OBJECT::VAO::generate::this object is already generated" << std::endl;
		}
	}

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;
	~VAO() { kill(); }
};

#endif // !VAO

