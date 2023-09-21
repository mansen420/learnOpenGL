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
    sampler2D ambient;
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
    vec3 diffuseMap = vec3(texture(defaultMaterial.diffuse, textureCoordinates));
    vec3 specMap = vec3(texture(defaultMaterial.specular, textureCoordinates));
    //ambient 
    vec3 ambient = cubeLight.ambient;
    //diffuse
    //spot light calculations
    vec3 diffuse = vec3(0.0);
    vec3 spotDirection = normalize(flashLight.position-fragPosition);
    float spotRelativeDirection = dot(spotDirection, normalize(-flashLight.direction));  //this returns the cosine of the angle between the two vectors
    if (spotRelativeDirection>flashLight.cutOff)       //angle1 > angle2  <->  cos(angle1) < cos(angle2) for angles<90
    {
        diffuse += flashLight.diffuse;   
    }
    else
    {
        float outerCutOff =cos(radians(7.0));
        float epsilon =  abs(outerCutOff - flashLight.cutOff);
        float intensity = clamp((spotRelativeDirection-outerCutOff)/epsilon, 0.0, 1.0);
        diffuse += flashLight.diffuse * intensity;
    }
    //cube light calculations
    vec3 cubeDirection = normalize(cubeLight.position-fragPosition);
    float cubeNormalFactor = max(dot(cubeDirection, normalize(normal)), 0.0);
    diffuse += cubeNormalFactor * cubeLight.diffuse;
    //specular
    vec3 camDirection = normalize(camPos-fragPosition);
    vec3 reflectDir = reflect(-cubeDirection, normalize(normal));
    float spec = pow(max(dot(camDirection, reflectDir),0.0), defaultMaterial.shininess);
    vec3 specular = spec * cubeLight.specular;

    vec3 result = (((ambient+diffuse)*diffuseMap)+(specular*specMap)) * attenutation;     //how much light to reflect * color of the object * attenutation
    FragColor = vec4(result, 1.0);
}