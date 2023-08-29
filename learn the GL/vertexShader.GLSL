#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 ourColor;
void main()
{
	ourColor = vec4(aPos, 1);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}