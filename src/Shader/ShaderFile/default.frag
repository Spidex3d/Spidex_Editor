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

uniform sampler2D objectTexture; // <-- Only one texture per object

// ====== MAIN ======
void main()
{
    vec3 norm = normalize(Normal);
    vec3 albedo = texture(objectTexture, TexCoords).rgb;

    // ===== Ambient + Sun light =====
    vec3 result = 0.1 * albedo;

    vec3 lightDir = normalize(-SunLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    result += SunLight.color * diff * SunLight.intensity * albedo;

    // ===== Point lights =====
    for (int i = 0; i < numPointLights; ++i) {
        vec3 toLight = PointLights[i].position - FragPos;
        float dist = length(toLight);
        float atten = clamp(1.0 - dist / PointLights[i].radius, 0.0, 1.0);
        vec3 lightDir = normalize(toLight);
        float diff = max(dot(norm, lightDir), 0.0);
        result += PointLights[i].color * PointLights[i].intensity * diff * atten * albedo;
    }

    // ===== Area lights =====
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



//#version 460
//
//in vec2 TexCoord;
//out vec4 FragColor;
//
//uniform sampler2D baseColor;
////uniform sampler2D objTexture;
//
//void main()
//{
//	FragColor = texture(baseColor, TexCoord);
//	
//}







// Outputs colors in RGBA
// Input from vertex shader
//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;
//in vec3 Tangent;
//
//// Output color
//out vec4 FragColor;
//
//// Material Textures
//uniform sampler2D baseColorMap;       // Albedo/BaseColor map
//uniform sampler2D metallicRoughnessMap; // Metallic-Roughness map
//uniform sampler2D normalMap;          // Normal map
//uniform sampler2D emissiveMap;        // Emissive map (optional)
//
//// Lighting uniforms
//uniform vec3 lightPos;  // Light position
//uniform vec3 viewPos;   // Camera position
//uniform vec3 lightColor; // Light color
//
//// PBR constants
//const float PI = 3.14159265359;
//
//// Function to get normal map
//vec3 getNormalFromMap()
//{
//    vec3 tangentNormal = texture(normalMap, TexCoords).rgb;
//    tangentNormal = tangentNormal * 2.0 - 1.0; // Transform from [0,1] to [-1,1]
//
//    vec3 Q1 = dFdx(FragPos);
//    vec3 Q2 = dFdy(FragPos);
//    vec2 st1 = dFdx(TexCoords);
//    vec2 st2 = dFdy(TexCoords);
//
//    vec3 N = normalize(Normal);
//    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
//    vec3 B = -normalize(cross(N, T));
//    mat3 TBN = mat3(T, B, N);
//
//    return normalize(TBN * tangentNormal);
//}
//
//// Fresnel-Schlick approximation
//vec3 fresnelSchlick(float cosTheta, vec3 F0)
//{
//    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//}
//
//// Geometry function (Smith's method)
//float geometrySmith(float NdotV, float NdotL, float roughness)
//{
//    float r = roughness + 1.0;
//    float k = (r * r) / 8.0;
//
//    float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
//    float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
//
//    return ggx1 * ggx2;
//}
//
//// Distribution function (GGX Trowbridge-Reitz)
//float distributionGGX(vec3 N, vec3 H, float roughness)
//{
//    float a = roughness * roughness;
//    float a2 = a * a;
//    float NdotH = max(dot(N, H), 0.0);
//    float NdotH2 = NdotH * NdotH;
//
//    float num = a2;
//    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//    denom = PI * denom * denom;
//
//    return num / denom;
//}
//
//void main()
//{
//    // Retrieve material properties
//    vec4 baseColor = texture(baseColorMap, TexCoords);
//    vec3 albedo = baseColor.rgb;
//    float metallic = texture(metallicRoughnessMap, TexCoords).b;
//    float roughness = texture(metallicRoughnessMap, TexCoords).g;
//    vec3 emissive = texture(emissiveMap, TexCoords).rgb;
//
//    // Get normal from normal map
//    vec3 N = getNormalFromMap();
//    vec3 V = normalize(viewPos - FragPos); // View vector
//    vec3 L = normalize(lightPos - FragPos); // Light vector
//    vec3 H = normalize(V + L);             // Halfway vector
//
//    // Fresnel-Schlick approximation
//    vec3 F0 = vec3(0.04); // Default reflectance
//    F0 = mix(F0, albedo, metallic);
//    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
//
//    // Geometry function
//    float G = geometrySmith(max(dot(N, V), 0.0), max(dot(N, L), 0.0), roughness);
//
//    // Distribution function
//    float D = distributionGGX(N, H, roughness);
//
//    // Cook-Torrance BRDF
//    vec3 numerator = D * G * F;
//    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // Prevent divide by zero
//    vec3 specular = numerator / denominator;
//
//    // kS is the specular component, kD is the diffuse component
//    vec3 kS = F;
//    vec3 kD = 1.0 - kS;
//    kD *= 1.0 - metallic;
//
//    // Diffuse (Lambertian)
//    float NdotL = max(dot(N, L), 0.0);
//    vec3 diffuse = kD * albedo / PI;
//
//    // Final color
//    vec3 radiance = lightColor * NdotL;
//    vec3 color = (diffuse + specular) * radiance;
//
//    // Add emissive contribution
//    color += emissive;
//
//    FragColor = vec4(color, baseColor.a);
//   // FragColor = vec4(TexCoords, 0.0, 1.0);
//}

