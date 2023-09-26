#ifndef CAMERA
#define CAMERA
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MISC_DECLR.h"

class Camera
{	
public:		//temporary
	GLFWwindow* window;
	glm::mat4* view;

	float framerateVariable = 0.0f;
	float lastFrame = 0.0f;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);		//view will be here at start of render 
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		float cameraSpeed = 2.5 * framerateVariable;
	//	cameraFront.y = 0;			//keeps user on ground. (why doesnt it effect the direction?)
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPosition += cameraFront * cameraSpeed;		//cameraFront is always normalized
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPosition -= cameraFront * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	}
public:
	Camera(GLFWwindow* window, glm::mat4* viewMatrix)
	{
		view = viewMatrix;
		this->window = window;
	}
	void update()
	{
		processInput();
		//update framerate variable
		framerateVariable = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		//update camera facing 
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
		*view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	}
};
#endif // !CAMERA