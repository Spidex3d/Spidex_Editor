#version 460

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D baseColor;
uniform sampler2D normalMap;
uniform sampler2D metallicRoughnessMap;
uniform sampler2D occlusionMap;
uniform sampler2D emissiveMap;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    // Base color
    vec4 base = texture(baseColor, texCoords);

     // Normal mapping
    vec3 norm = normalize(normal);
    vec3 normalTex = texture(normalMap, texCoords).rgb;
    normalTex = normalize(normalTex * 2.0 - 1.0);
    //norm = normalize(normalTex); // override default normal for now

    // Metallic-Roughness
    vec3 mrTex = texture(metallicRoughnessMap, texCoords).rgb;
    float roughness = mrTex.g;
    float metallic = mrTex.b;

     // Occlusion
    float ao = texture(occlusionMap, texCoords).r;

     // Emissive
    vec3 emissive = texture(emissiveMap, texCoords).rgb;

    // Lighting (very basic Blinn-Phong)
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0 * (1.0 - roughness));
    vec3 lighting = (diff + spec) * lightColor * ao;

    vec3 finalColor = base.rgb * lighting + emissive;

    FragColor = vec4(finalColor, base.a);
    //FragColor = texture(baseColor, texCoords);
}





//in vec2 TexCoords;
//in vec3 FragPos;
//in vec3 Normal;
//
//out vec4 FragColor;
////  "baseColor", "normal", "metallicRoughness", "occlusion", "emissive"
//// Samplers for glTF-supported textures
//uniform sampler2D baseColor;
//uniform sampler2D normal;
//uniform sampler2D metallicRoughness;
//uniform sampler2D occlusion;
//uniform sampler2D emissive;
//
//// Lighting
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//void main() {
////    vec3 albedo = texture(baseColor, TexCoords).rgb;
////
////    vec3 normalMap = texture(normal, TexCoords).rgb;
////    normalMap = normalMap * 2.0 - 1.0; // convert from [0,1] to [-1,1]
////    vec3 N = normalize(normalMap);
////    //vec3 N = normalize(Normal); // Add normal map if needed
////    vec3 L = normalize(lightPos - FragPos);
////    vec3 V = normalize(viewPos - FragPos);
////    vec3 H = normalize(L + V);
////
////    // Metallic and roughness from red and green channels
////    vec2 mr = texture(metallicRoughness, TexCoords).rg;
////    float metallic = mr.r;
////    float roughness = mr.g;
////
////    // Simple diffuse shading
////    float diff = max(dot(N, L), 0.0);
////
////    // Occlusion multiplier
////    float ao = texture(occlusion, TexCoords).r;
////
////    // Emissive color
////    vec3 emissiveColor = texture(emissive, TexCoords).rgb;
////
////    // Combine everything (placeholder PBR)
////    vec3 color = (albedo * diff * ao) + emissiveColor;
////
////    FragColor = vec4(color, 1.0);
//
// //   FragColor = vec4(TexCoords, 0.0, 1.0);
//
//// vec3 albedo = texture(baseColor, TexCoords).rgb;
////    FragColor = vec4(albedo, 1.0);
////
//vec4 base = texture(baseColor, TexCoords);
//    // Sample others if needed
//    FragColor = base; // At least do this to make sure `baseColor` is actually used
//}
//

//in vec2 TexCoord;
//out vec4 FragColor;
//
//uniform sampler2D texture_diffuse;
//
//void main() {
//
//  FragColor = texture(texture_diffuse, TexCoord);
//}





//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//
// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_specular1;
//
//void main()
//{
//    vec4 diffuse = texture(texture_diffuse1, TexCoords);
//    vec4 specular = texture(texture_specular1, TexCoords);
//   
//   FragColor = diffuse + specular;
//        
//}

//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;  // UV coordinates from the vertex shader
//
//out vec4 FragColor; // Output color
//
////uniform sampler2D texture_diffuse;        
//uniform sampler2D texture_diffuse1;        
//uniform sampler2D texture_specular1;        
//uniform sampler2D texture_normal1;        
//uniform sampler2D texture_height1;        
//
//
//void main() {
//
//     //FragColor = vec4(0.7f, 0.7f, 0.0f, 1.0f);
//    // FragColor = vec4(TexCoords, 0.0, 1.0);
//    FragColor = texture(texture_diffuse1, TexCoords); // Sample texture color at UV coordinates
//}