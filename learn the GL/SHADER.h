#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        //1. read shaders from file and get c style strings
        std::string vertexCode, fragmentCode;
        std::ifstream vFile, fFile;
        vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vFile.open(vertexPath), fFile.open(fragmentPath);
            std::stringstream vStream, fStream;
            vStream << vFile.rdbuf(), fStream << fFile.rdbuf();
            vFile.close(), fFile.close();
            vertexCode = vStream.str(), fragmentCode = fStream.str();
        }
        catch (std::ifstream::failure excp)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* cStrVertexCode = vertexCode.c_str(); 
        const char* cStrFragmentCode = fragmentCode.c_str();
        //2. compile shaders
        int  success;
        char infoLog[512];
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &cStrVertexCode, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &cStrFragmentCode, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() { glUseProgram(ID); }
};

#endif
