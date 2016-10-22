#version 330
in vec4 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 fColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

vec3 norm = normalize(vNormal);
vec3 lightDir = normalize(lightPos - vFragPos);

vec3 viewDir = normalize(viewerPos - vFragPos);
vec3 reflectDir = reflect(-lightDir, norm);

float ambient = 0.1f;
float diffuse = max(dot(norm, lightDir), 0.0f);
float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32);

void main()
{
   fColor = vColor * vec4(lightColor * (ambient  + diffuse + specular), 1.0);
//    fColor = vec4(0, 0, 0.3f, 1);
}
