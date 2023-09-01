#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "SHADER.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL_OBJECT.h"


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* myWindow = glfwCreateWindow(800, 800, "openGL window",NULL, NULL);
    if (myWindow == NULL)
    {
        std::cout << "FAILED TO CREATE WINDOW" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(myWindow);
    glfwSetFramebufferSizeCallback(myWindow, framebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 800);
    glClearColor(0.7f, 0.8f, 1.0f, 1.0f);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    VAO vao0(true);
    vao0.bind(); 
    vao0.bindVBO(), vao0.bindEBO();
    vao0.vertexBufferData(vertices, GL_STATIC_DRAW, sizeof(vertices));
    vao0.elementBufferData(indices, GL_STATIC_DRAW, sizeof(indices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shaders("C:/Users/msi/source/repos/learn the GL/learn the GL/vertexShader.GLSL",
                   "C:/Users/msi/source/repos/learn the GL/learn the GL/fragmentShader.GLSL");

    while (!glfwWindowShouldClose(myWindow))
    {
        processInput(myWindow);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaders.ID);
        glBindVertexArray(vao0.getID());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}