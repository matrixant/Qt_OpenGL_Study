#version 330
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specShininess;
};

struct Light {
    vec3 pos;
//    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec4 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 fColor;

uniform Material material;
uniform Light light;

//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewerPos;

vec3 norm = normalize(vNormal);
vec3 lightDir = normalize(light.pos - vFragPos);

vec3 viewDir = normalize(viewerPos - vFragPos);
vec3 reflectDir = reflect(-lightDir, norm);

vec3 ambient = light.ambient * material.ambient;
float diff = max(dot(norm, lightDir), 0.0f);
vec3 diffuse = light.diffuse * material.diffuse * diff;
float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.specShininess);
vec3 specular = light.specular * material.specular * spec;

void main()
{
   fColor = vColor * vec4(ambient  + diffuse + specular, 1.0);
//    fColor = vec4(0, 0, 0.3f, 1);
}
