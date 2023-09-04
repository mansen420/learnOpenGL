#version 330 core
out vec4 FragColor;

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

uniform sampler2D texture0;

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(0.3,0.5,0.9);

uniform vec3 lightPos;
uniform vec3 camPos;
void main()
{
    //ambient 
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //diffuse
    vec3 direction = normalize(lightPos-fragPosition);
    float normalFactor = max(dot(direction, normalize(normal)), 0.0);
    vec3 diffuse = normalFactor*lightColor;
    //specular
    float specFactor = 0.3;
    vec3 camDirection = normalize(camPos-fragPosition);
    vec3 reflectDir = reflect(-direction, normalize(normal));
    float spec = pow(max(dot(camDirection, reflectDir),0.0),32.0);
    vec3 specular = specFactor*spec*lightColor;

    vec3 result = ambient+diffuse+specular;      //how much light to reflect 
    FragColor = vec4(result * objectColor, 1.0);
}