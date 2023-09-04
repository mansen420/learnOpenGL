#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textCoords;

uniform mat4 view, model, projection;

out vec2 textureCoordinates;
void main()
{
	gl_Position = projection * view * model* vec4(aPos.x, aPos.y, aPos.z, 1.0);
	textureCoordinates = textCoords;
}