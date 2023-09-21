#ifndef MISC
#define MISC
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWwindow* init();
//global variables, defined in MISC_DEF
extern const int screenWidth, screenHeight;
extern float fov;
extern bool firstMouse;
extern float lastX, lastY;
extern float mouseSensitivity;
extern float yaw, pitch;

#endif // !MISC

