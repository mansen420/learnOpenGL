#version 330 core
out vec4 FragColor;

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

uniform vec3 camPos;

struct Material
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
//magic constants 
float constantFactor= 1.0;
float linearFactor = 0.09; 
float quadraticFactor = 0.032;
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};
uniform Light cubeLight;
uniform Material defaultMaterial;

float lightDistane = length(fragPosition-cubeLight.position);
float attenutation = 1.0/(lightDistane*lightDistane*quadraticFactor + lightDistane*linearFactor + constantFactor);
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
    vec3 specMap = vec3(texture(defaultMaterial.specular, textureCoordinates));
    vec3 specular = spec * specMap * cubeLight.specular;

    vec3 result = (ambient+diffuse+specular) * attenutation;     //how much light to reflect * color of the object 
    FragColor = vec4(result, 1.0);
}