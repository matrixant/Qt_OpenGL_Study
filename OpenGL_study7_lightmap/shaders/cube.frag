#version 330
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float specShininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuatConstant;
    float attenuatLinear;
    float attenuatQuadratic;
};

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

uniform Material material;
uniform PointLight pointLight;

uniform vec3 viewerPos;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 norm = normalize(normal);
    vec3 lightDir = light.position - fragPos;
    float distance = length(lightDir);

    lightDir = normalize(lightDir);
    vec3 reflectDir = reflect(-lightDir, norm);

    float lightAttenuation = 1.0f / (light.attenuatConstant + light.attenuatLinear * distance + light.attenuatQuadratic * (distance * distance));

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.specShininess) * 0.5f;

    vec3 ambient = light.ambient * vec3(texture2D(material.diffuse, vTexCoord)) * lightAttenuation;
    vec3 diffuse = light.diffuse * vec3(texture2D(material.diffuse, vTexCoord)) * diff * lightAttenuation;
    vec3 specular = light.specular * vec3(texture2D(material.specular, vTexCoord)) * spec * lightAttenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 result = calcPointLight(pointLight, vNormal, vFragPos, normalize(viewerPos - vFragPos));
//    gl_FragColor = texture2D(material.specular, vTexCoord);
    gl_FragColor = vec4(result, 1.0f);
}
