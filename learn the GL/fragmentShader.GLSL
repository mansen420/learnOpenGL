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
    float lightDistane = length(fragPosition-cubeLight.position);
    float attenutation = 1.0/(lightDistane*lightDistane*quadraticFactor + lightDistane*linearFactor + constantFactor);
    //lightmaps
    vec3 diffuseMap = vec3(texture(defaultMaterial.texture_diffuse1, textureCoordinates));
    vec3 specMap = vec3(texture(defaultMaterial.texture_specular1, textureCoordinates));
    //ambient 
    vec3 ambient = cubeLight.ambient;
    //diffuse
    //spot light calculations
    vec3 lightDirection= vec3(-0.2,-1.0,-0.2);
    vec3 diffuse = vec3(0.0);
    diffuse +=  vec3(0.7,0.7,0.7)*max(dot(normalize(-lightDirection), normalize(normal)), 0.0);

    FragColor = vec4((diffuse)*diffuseMap, 1.0);
}