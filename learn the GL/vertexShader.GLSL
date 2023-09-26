#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;			//the vertex shader expects this layout
layout (location = 2) in vec2 textCoords;		//cross check with any calls to glVertexAttribPointer()

uniform mat4 view, model, projection;			

out vec2 textureCoordinates;
out vec3 fragPosition;
out vec3 normal;
void main()
{
	gl_Position = projection * view * model* vec4(aPos.x, aPos.y, aPos.z, 1.0);
	textureCoordinates = textCoords;
	normal = mat3(transpose(inverse(model))) * aNormal;
	fragPosition = vec3( model * vec4(aPos,1.0) );
}