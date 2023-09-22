#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "SHADER.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL_OBJECT.h"
#include "CAMERA.h"
#include "MISC_DECLR.h"

int main()
{
    GLFWwindow* myWindow = init();
    float vertices[] = {
     //vertex             //texture   //normal 
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };
    //buffer object instantiation and vertex attribute configuration 
    VAO vao0(true);
    vao0.bind(); 
    vao0.bindVBO();
    vao0.vertexBufferData(vertices, GL_STATIC_DRAW, sizeof(vertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    VAO vao1LIGHT(true);
    vao1LIGHT.bind();
    vao1LIGHT.overwriteVBO(&vao0.vertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //texture loading 
    stbi_set_flip_vertically_on_load(true);
    int height, width, nrChannels;
    unsigned char* data = stbi_load("C:/Users/msi/Desktop/container2.PNG", &width, &height, &nrChannels, 0);

    unsigned int texture0;
    if (data)
    {
        glGenTextures(1, &texture0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "loading texture failed" << std::endl;
    data = 0;
    data = stbi_load("C:/Users/msi/Desktop/container2_specular.PNG", &width, &height, &nrChannels, 0);
    unsigned int texture1;
    if (data)
    {
        glGenTextures(1, &texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "loading texture failed" << std::endl;

    data = 0;
    data = stbi_load("C:/Users/msi/Desktop/matrix.JPG", &width, &height, &nrChannels, 0);
    unsigned int texture2;
    if (data)
    {
        glGenTextures(1, &texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "loading texture failed" << std::endl;

    //shader configuration 
    Shader shaders("C:/Users/msi/source/repos/learn the GL/learn the GL/vertexShader.GLSL",
                   "C:/Users/msi/source/repos/learn the GL/learn the GL/fragmentShader.GLSL");
    shaders.use();      //use() before setting uniforms
    //object material values
    glUniform1f(glGetUniformLocation(shaders.ID, "defaultMaterial.shininess"), 32.0f);
    //light values
    glUniform3f(glGetUniformLocation(shaders.ID, "cubeLight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaders.ID, "cubeLight.diffuse"), 0.7f, 0.7f, 0.7f);
    glUniform3f(glGetUniformLocation(shaders.ID, "cubeLight.specular"), 1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shaders.ID, "flashLight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaders.ID, "flashLight.diffuse"), 0.7f, 0.7f, 0.7f);
    glUniform3f(glGetUniformLocation(shaders.ID, "flashLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaders.ID, "flashLight.cutOff"), glm::cos(glm::radians(5.0f)));
    Shader shaderLIGHT("C:/Users/msi/source/repos/learn the GL/learn the GL/vertexShader.GLSL",
        "C:/Users/msi/source/repos/learn the GL/fragmentShaderLIGHT.GLSL");

    //render loop
    glm::mat4 view = glm::mat4(1.0f);
    Camera cam(myWindow, &view);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(myWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4  model, projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.0f);


        glUseProgram(shaders.ID);
        glm::vec3 lightPos(12.0f * cos(0.3f*glfwGetTime()), 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaders.ID, "cubeLight.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shaders.ID, "flashLight.position"), cam.cameraPosition.x, cam.cameraPosition.y, cam.cameraPosition.z);
        glUniform3f(glGetUniformLocation(shaders.ID, "flashLight.direction"), cam.cameraFront.x, cam.cameraFront.y, cam.cameraFront.z);

        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao0.getID());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shaderLIGHT.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  //must send uniforms for each program 
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao1LIGHT.getID());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cam.update();
        shaders.use();
        glUniform3f(glGetUniformLocation(shaders.ID, "camPos"), cam.cameraPosition.x, cam.cameraPosition.y, cam.cameraPosition.z);

        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}