#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "SHADER.h"
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL_OBJECT.h"
#include "CAMERA.h"
#include "MISC_DECLR.h"
#include "MODEL.h"

int main()
{
    GLFWwindow* myWindow = init();
    stbi_set_flip_vertically_on_load(true);
    //shader configuration 
    Shader shaders("C:/Users/msi/source/repos/learn the GL/learn the GL/vertexShader.GLSL",
        "C:/Users/msi/source/repos/learn the GL/learn the GL/fragmentShader.GLSL");
    Shader shaderLIGHT("C:/Users/msi/source/repos/learn the GL/learn the GL/vertexShader.GLSL",
        "C:/Users/msi/source/repos/learn the GL/fragmentShaderLIGHT.GLSL");
    //Testing
 
    Model backpack("C:/Users/msi/Desktop/backpack/backpack.obj");

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
        backpack.draw(shaders);

      //  glDrawArrays(GL_TRIANGLES, 0, 36);


        shaderLIGHT.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  //must send uniforms for each program 
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderLIGHT.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        cam.update();
        shaders.use();
        glUniform3f(glGetUniformLocation(shaders.ID, "camPos"), cam.cameraPosition.x, cam.cameraPosition.y, cam.cameraPosition.z);

        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}