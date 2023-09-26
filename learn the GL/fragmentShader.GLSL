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

    float shininess;
};
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;      //for point light and spotlight
    float cutOff;       //for spotlight
    vec3 direction;     //for directional light
};

uniform Material defaultMaterial;
uniform Light sunlight;
uniform vec3 cam_pos;

void main()
{
    //light types 
    vec3 diffuse, ambient, specular = vec3(0.0);
    //light maps
    vec3 diffuse_map = vec3(texture(defaultMaterial.texture_diffuse1, textureCoordinates));
    vec3 specular_map = vec3(texture(defaultMaterial.texture_specular1, textureCoordinates));
    //ambient calculations 
    ambient += sunlight.ambient;
    //directional diffuse calculations
    float normal_factor = max(dot(normalize(-sunlight.direction),normalize(normal)),0.0);
    diffuse += normal_factor*sunlight.diffuse;
    //specular calculations 
    vec3 cam_direction = normalize(cam_pos-fragPosition);
    vec3 reflect_direction = reflect(normalize(-sunlight.direction), normalize(normal));
    float spec = pow(max(dot(cam_direction, reflect_direction),0.0), defaultMaterial.shininess);
    specular += spec*sunlight.specular;

    vec3 result = ((diffuse+ambient)*diffuse_map)+(specular*specular_map);
    FragColor = vec4(result, 1.0);
}