#version 460 core
out vec4 FragColor;

#define NB_TEXTURES 4

struct Material{
    sampler2D texture_diffuse[NB_TEXTURES];
    sampler2D texture_specular[NB_TEXTURES];
    sampler2D texture_normal[NB_TEXTURES];
    sampler2D texture_height[NB_TEXTURES];

    float shininess;
};

struct DirLight{ 
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform DirLight dirLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures);
void ProcessTextures(inout vec3 diffuse_textures, inout vec3 specular_textures);

void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 diffuse_textures;
    vec3 specular_textures;
    ProcessTextures(diffuse_textures, specular_textures);

    vec3 result = vec3(0.0);
    result += CalcDirLight(dirLight, norm, viewDir, diffuse_textures, specular_textures);
    // for (int i = 0; i < NR_POINT_LIGHTS; i++){
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffuse_textures, specular_textures);
    // }
    for (int i = 0; i < NR_SPOT_LIGHTS; i++){
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, diffuse_textures, specular_textures);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures){
    vec3 lightDir = normalize(-light.direction);
    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //total
    vec3 ambient = light.ambient * diffuse_textures;
    vec3 diffuse = light.diffuse * diff * diffuse_textures;
    vec3 specular = light.specular * spec * specular_textures;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures){
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                                light.quadratic * (dist * dist));
    //total
    vec3 ambient = light.ambient * diffuse_textures * attenuation;
    vec3 diffuse = light.diffuse * diff * diffuse_textures * attenuation;
    vec3 specular = light.specular * spec * specular_textures * attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse_textures, vec3 specular_textures){
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                                light.quadratic * (dist * dist));
    //spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //total
    vec3 ambient = light.ambient * diffuse_textures * attenuation * intensity;
    vec3 diffuse = light.diffuse * diff * diffuse_textures * attenuation * intensity;
    vec3 specular = light.specular * spec * specular_textures * attenuation * intensity;
    return (ambient + diffuse + specular);
}

void ProcessTextures(inout vec3 diffuse_textures, inout vec3 specular_textures){
    diffuse_textures = texture(material.texture_diffuse[0], TexCoords).rgb;
    specular_textures = texture(material.texture_specular[0], TexCoords).rgb;
    for (int i = 1; i < NB_TEXTURES; i++){
        if (textureSize(material.texture_diffuse[i], 0).x > 1){
            diffuse_textures += texture(material.texture_diffuse[i], TexCoords).rgb;
            diffuse_textures /= 2;
        }
        if (textureSize(material.texture_specular[i], 0).x > 1){
            specular_textures += texture(material.texture_specular[i], TexCoords).rgb;
            specular_textures /= 2;
        }
    }
}