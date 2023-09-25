#version 330 core
out vec4 FragColor;

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

uniform vec3 camPos;

//magic constants 
float constantFactor= 1.0;
float linearFactor = 0.09; 
float quadraticFactor = 0.032;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    float shininess;
};
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;

    float cutOff;
    vec3 direction;
};

uniform Light cubeLight;
uniform Light flashLight;
uniform Material defaultMaterial;

void main()
{
    FragColor = vec4(1.0,0.0,0.0, 1.0);
}