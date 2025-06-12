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
uniform float tiling = 1.0; // 10
uniform float minHeight;
uniform float maxHeight;
uniform float heightBlendRange;
uniform float slopeStart;
uniform float slopeEnd;

// ====== MAIN ======
void main()
{
    vec3 norm = normalize(Normal);

    // -------- Sample blend map --------
    vec4 blend = texture(blendMap, TexCoords);
    float total = blend.r + blend.g + blend.b;
    float remaining = clamp(1.0 - total, 0.0, 1.0);  // Remaining is grass

    // -------- Sample base textures --------
    vec3 grass = texture(texture_grass, TexCoords * tiling).rgb;
    vec3 rock  = texture(texture_rock,  TexCoords * tiling).rgb;
    vec3 dirt  = texture(texture_dirt,  TexCoords * tiling).rgb;

    // -------- Height-based blend factor (for rock) --------
    float heightNorm = clamp((FragPos.y - minHeight) / (maxHeight - minHeight), 0.0, 1.0);
    float heightRockBlend = smoothstep(0.5 - heightBlendRange, 0.5 + heightBlendRange, heightNorm);
    // -------- Slope-based rock blending --------
    float slope = dot(norm, vec3(0.0, 1.0, 0.0)); // 1.0 = flat, 0.0 = vertical
    float slopeRockBlend = smoothstep(slopeEnd, slopeStart, slope); // More rock as slope decreases

    // -------- Combine painted blend with height blending --------
    float paintedRock = blend.r;
    float paintedDirt = blend.g;
    float paintedGrass = remaining + blend.b; // blend.b was also used for grass

    // Boost rock layer by height-based factor
    //float finalRock = clamp(paintedRock + heightRockBlend * (1.0 - paintedRock), 0.0, 1.0);

    float combinedRockBoost = max(heightRockBlend, slopeRockBlend); // choose stronger of the two
    float finalRock = clamp(paintedRock + combinedRockBoost * (1.0 - paintedRock), 0.0, 1.0);

    // Reduce grass and dirt to make room
    float adjust = finalRock - paintedRock;
    float finalDirt = clamp(paintedDirt - adjust * 0.5, 0.0, 1.0);
    float finalGrass = clamp(paintedGrass - adjust * 0.5, 0.0, 1.0);

    // Normalize (optional but improves blending safety)
    float sum = finalGrass + finalRock + finalDirt;
    vec3 weights = (sum > 0.0) ? vec3(finalGrass, finalRock, finalDirt) / sum : vec3(1, 0, 0);

    vec3 albedo = grass * weights.x + rock * weights.y + dirt * weights.z;

    // -------- Lighting (unchanged) --------
    vec3 result = 0.1 * albedo;

    vec3 lightDir = normalize(-SunLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    result += SunLight.color * diff * SunLight.intensity * albedo;

    for (int i = 0; i < numPointLights; ++i) {
        vec3 toLight = PointLights[i].position - FragPos;
        float dist = length(toLight);
        float atten = clamp(1.0 - dist / PointLights[i].radius, 0.0, 1.0);
        vec3 lightDir = normalize(toLight);
        float diff = max(dot(norm, lightDir), 0.0);
        result += PointLights[i].color * PointLights[i].intensity * diff * atten * albedo;
    }

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






//void main()
//{
//
//    vec3 norm = normalize(Normal);
//
//    // -------- Multi-texture blending --------
//    vec4 blend = texture(blendMap, TexCoords);
//    float total = blend.r + blend.g + blend.b;
//    float remaining = clamp(1.0 - total, 0.0, 1.0);  // unused weight goes to base (e.g., grass)
//
//
//    // Sample textures
//    vec3 grass = texture(texture_grass, TexCoords * tiling).rgb;
//    vec3 rock  = texture(texture_rock,  TexCoords * tiling).rgb;
//    vec3 dirt  = texture(texture_dirt,  TexCoords * tiling).rgb;
//
//    vec3 albedo = remaining * grass + blend.r * rock + blend.g * dirt + blend.b * grass;
//   
//    // -------- End Texture blending --------
//
//    vec3 result = 0.1 * albedo;
//
//    // -------- Sun (directional light)
//    vec3 lightDir = normalize(-SunLight.direction);
//    float diff = max(dot(norm, lightDir), 0.0);
//    result += SunLight.color * diff * SunLight.intensity * albedo;
//
//    // -------- Point lights
//    for (int i = 0; i < numPointLights; ++i) {
//        vec3 toLight = PointLights[i].position - FragPos;
//        float dist = length(toLight);
//        float atten = clamp(1.0 - dist / PointLights[i].radius, 0.0, 1.0);
//        vec3 lightDir = normalize(toLight);
//        float diff = max(dot(norm, lightDir), 0.0);
//
//        result += PointLights[i].color * PointLights[i].intensity * diff * atten * albedo;
//    }
//
//    // -------- Area lights
//    for (int i = 0; i < numAreaLights; ++i) {
//        vec3 toALight = AreaLights[i].position - FragPos;
//        float dist = length(toALight);
//        float atten = pow(clamp(1.0 - dist / AreaLights[i].area, 0.0, 1.0), 2.0);
//        vec3 lightDir = normalize(toALight);
//        float diff = max(dot(norm, lightDir), 0.0);
//
//        result += AreaLights[i].color * AreaLights[i].intensity * diff * atten * albedo;
//    }
//
//    FragColor = vec4(result, 1.0);
//}





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


