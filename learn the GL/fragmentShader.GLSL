#version 330 core
out vec4 FragColor;

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

uniform vec3 camPos;

struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};
uniform Light cubeLight;
uniform Material defaultMaterial;

void main()
{
    //ambient 
    vec3 ambient = cubeLight.ambient * vec3(texture(defaultMaterial.diffuse, textureCoordinates));
    //diffuse
    vec3 direction = normalize(cubeLight.position-fragPosition);
    float normalFactor = max(dot(direction, normalize(normal)), 0.0);
    vec3 diffuse = normalFactor * cubeLight.diffuse * vec3(texture(defaultMaterial.diffuse, textureCoordinates));
    //specular
    vec3 camDirection = normalize(camPos-fragPosition);
    vec3 reflectDir = reflect(-direction, normalize(normal));
    float spec = pow(max(dot(camDirection, reflectDir),0.0), defaultMaterial.shininess);
    vec3 specular = spec * vec3(texture(defaultMaterial.specular, textureCoordinates)) * cubeLight.specular;

    vec3 result = ambient+diffuse+specular;      //how much light to reflect * color of the object 
    FragColor = vec4(result, 1.0);
}