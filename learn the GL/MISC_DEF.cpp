#include "MISC_DECLR.h"

const int screenWidth{ 800 }, screenHeight{ 800 };
float fov = 45.0f;
bool firstMouse = true;
float lastX{ screenWidth / 2 }, lastY{ screenHeight / 2 };
float mouseSensitivity = 0.1f;
float yaw{ -90.0f }, pitch{ 0.0f };

GLFWwindow* init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* myWindow = glfwCreateWindow(screenWidth, screenHeight, "openGL window", NULL, NULL);
    if (myWindow == NULL)
    {
        std::cout << "FAILED TO CREATE WINDOW" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(myWindow);
    glfwSetFramebufferSizeCallback(myWindow, framebufferSizeCallback);
    glfwSetCursorPosCallback(myWindow, mouse_callback);
    glfwSetScrollCallback(myWindow, scroll_callback);
    glfwSetInputMode(myWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    return myWindow;
}
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset, yOffset;

    xOffset = xPos - lastX;
    lastX = xPos;
    yOffset = -(yPos - lastY);		//positive yPos goes downward
    lastY = yPos;

    pitch += mouseSensitivity * yOffset;
    yaw += mouseSensitivity * xOffset;

    //limits such that the camera doesn't confuse itself
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}