#version 460
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// ====== LIGHT STRUCTS ======
struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

struct AreaLight {
    vec3 position;
    vec3 color;
    float intensity;
    float area;
};

// ====== UNIFORMS ======
uniform DirectionalLight SunLight;
uniform PointLight PointLights[10];
uniform AreaLight AreaLights[10];
uniform int numPointLights;
uniform int numAreaLights;
// ##### Texture blending #####
uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_dirt;
uniform sampler2D blendMap;

uniform float tiling = 10.0;

// ====== MAIN ======
void main()
{
    vec3 norm = normalize(Normal);

    // -------- Multi-texture blending --------
    vec4 blend = texture(blendMap, TexCoords);
    vec2 tiledUV = TexCoords * tiling;

    vec3 grass = texture(texture_grass, tiledUV).rgb;
    vec3 rock  = texture(texture_rock, tiledUV).rgb;
    vec3 dirt  = texture(texture_dirt, tiledUV).rgb;

    vec3 albedo = blend.r * grass + blend.g * rock + blend.b * dirt;
    // -------- End blending --------

    vec3 result = 0.1 * albedo;

    // -------- Sun (directional light)
    vec3 lightDir = normalize(-SunLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    result += SunLight.color * diff * SunLight.intensity * albedo;

    // -------- Point lights
    for (int i = 0; i < numPointLights; ++i) {
        vec3 toLight = PointLights[i].position - FragPos;
        float dist = length(toLight);
        float atten = clamp(1.0 - dist / PointLights[i].radius, 0.0, 1.0);
        vec3 lightDir = normalize(toLight);
        float diff = max(dot(norm, lightDir), 0.0);

        result += PointLights[i].color * PointLights[i].intensity * diff * atten * albedo;
    }

    // -------- Area lights
    for (int i = 0; i < numAreaLights; ++i) {
        vec3 toALight = AreaLights[i].position - FragPos;
        float dist = length(toALight);
        float atten = pow(clamp(1.0 - dist / AreaLights[i].area, 0.0, 1.0), 2.0);
        vec3 lightDir = normalize(toALight);
        float diff = max(dot(norm, lightDir), 0.0);

        result += AreaLights[i].color * AreaLights[i].intensity * diff * atten * albedo;
    }

    FragColor = vec4(result, 1.0);
}





//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;
//
//out vec4 FragColor;
//// using this as a sun
//struct DirectionalLight {
//    vec3 direction;
//    vec3 color;
//    float intensity;
//};
//
//uniform int numPointLights;
// struct PointLight{
//    vec3 position;
//    vec3 color;
//    float intensity;
//    float radius;
//};
//
//uniform int numAreaLights;
//struct AreaLight {
//vec3 position;
//vec3 color;
//float intensity;
//float area;
//};
//
//// Uniforms
//uniform sampler2D texture_diffuse;
//uniform DirectionalLight SunLight;
//uniform PointLight PointLights[10];
//uniform AreaLight AreaLights[10];
//
//void main()
//{
//    vec3 norm = normalize(Normal);
//    vec3 albedo = texture(texture_diffuse, TexCoords).rgb;
//   
//    // Start with ambient
//    vec3 result = 0.1 * albedo;
//
//    // Sun light (directional)
//    vec3 lightDir = normalize(-SunLight.direction);
//    float diff = max(dot(norm, lightDir), 0.0);
//    result += SunLight.color * diff * SunLight.intensity * albedo;
//
//    // Point lights
//    for (int i = 0; i < numPointLights; ++i) {
//        vec3 toLight = PointLights[i].position - FragPos;
//        float distance = length(toLight);
//        float attenuation = clamp(1.0 - distance / PointLights[i].radius, 0.0, 1.0);
//        vec3 lightDir = normalize(toLight);
//        float diff = max(dot(norm, lightDir), 0.0);
//
//        vec3 lightEffect = PointLights[i].color * PointLights[i].intensity * diff * attenuation;
//        result += lightEffect * albedo;
//    }
//
//    // Area lights
//    for (int i = 0; i < numAreaLights; ++i) {
//        vec3 toALight = AreaLights[i].position - FragPos;
//        float distance = length(toALight);
//        float attenuation = pow(clamp(1.0 - distance / AreaLights[i].area, 0.0, 1.0), 2.0);
//        vec3 lightDir = normalize(toALight);
//        float diff = max(dot(norm, lightDir), 0.0);
//
//        vec3 lightEffect = AreaLights[i].color * AreaLights[i].intensity * diff * attenuation;
//        result += lightEffect * albedo;
//    }
//
//    FragColor = vec4(result, 1.0);
//}


