#version 330 core
out vec4 FragColor;

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
};

uniform Material defaultMaterial;

void main()
{
    vec3 diffuse = vec3(texture(defaultMaterial.texture_diffuse1, textureCoordinates));
    vec3 result = diffuse;
    FragColor = vec4(result, 1.0);
}