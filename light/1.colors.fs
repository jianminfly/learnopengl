

#version 330 core
out vec4 FragColor;
  in vec3 Normal;
  uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
float ambientStrength = 0.1;
in vec3 FragPos;


void main()
{
    FragColor = vec4(ambientStrength*lightColor * objectColor, 1.0);

    vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

vec3 result = (ambientStrength + diffuse) * objectColor;
FragColor = vec4(result, 1.0);
}

