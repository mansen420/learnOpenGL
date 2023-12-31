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
    //uniform configuration 
    shaders.use();
    glUniform1f(glGetUniformLocation(shaders.ID, "defaultMaterial.shininess"), 32.0f);
    glUniform3f(glGetUniformLocation(shaders.ID, "sunlight.ambient"), 0.35f, 0.45f, 0.55f);
    glUniform3f(glGetUniformLocation(shaders.ID, "sunlight.diffuse"), 0.9f, 0.75f, 0.65f);
    glUniform3f(glGetUniformLocation(shaders.ID, "sunlight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaders.ID, "sunlight.direction"), 0.1, -1.0, -0.1);
    //model
    Model backpack("C:/Users/msi/Desktop/backpack/backpack.obj");

    //render loop 
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 10.0f));

    Camera cam(myWindow, &view);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(myWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4  model, projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.0f);
        glUseProgram(shaders.ID);
        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaders.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        backpack.draw(shaders);
        cam.update();
        glUniform3f(glGetUniformLocation(shaders.ID, "cam_pos"), cam.cameraPosition.x, cam.cameraPosition.y, cam.cameraPosition.z);

        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}