#version 330
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};

in vec3 vNormal;
in vec3 vFragPos;

uniform Material material;

const int POINTLIGHTS_NUM = 3;
uniform PointLight pointLight[POINTLIGHTS_NUM];
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
uniform int lightType;

uniform vec3 viewerPos;
//vec3 viewDir = normalize(viewerPos - fragPos);

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.specShininess) * 0.5f;

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse * diff;
    vec3 specular = light.specular * material.specular * spec;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 norm = normalize(normal);
    vec3 lightDir = light.position - fragPos;
    float distance = length(lightDir);

    lightDir = normalize(lightDir);
    vec3 reflectDir = reflect(-lightDir, norm);

    float lightAttenuation = 1.0f / (light.attenuatConstant + light.attenuatLinear * distance + light.attenuatQuadratic * (distance * distance));

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.specShininess) * 0.5f;

    vec3 ambient = light.ambient * material.ambient * lightAttenuation;
    vec3 diffuse = light.diffuse * material.diffuse * diff * lightAttenuation;
    vec3 specular = light.specular * material.specular * spec * lightAttenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(spotLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.specShininess) * 0.5f;

    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);

    vec3 ambient = spotLight.ambient * material.ambient;
    vec3 diffuse = spotLight.diffuse * material.diffuse * diff * intensity;
    vec3 specular = spotLight.specular * material.specular * spec * intensity;

    return ambient + diffuse + specular;
}

void main()
{
//    vec3 result = calcSpotLight(spotLight, vNormal, vFragPos, normalize(viewerPos - vFragPos));
//    result = calcDirectionalLight(dirLight, vNormal, vFragPos, normalize(viewerPos - vFragPos));

    vec3 result = vec3(0.0f);

    for(int i = 0; i < POINTLIGHTS_NUM; ++i) {
        result += calcPointLight(pointLight[i], vNormal, vFragPos, normalize(viewerPos - vFragPos));
    }

    gl_FragColor = vec4(result, 1.0);
}
