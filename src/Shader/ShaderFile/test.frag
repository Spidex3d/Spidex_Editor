#version 460
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;  // UV coordinates from the vertex shader

out vec4 FragColor; // Output color

uniform sampler2D baseColorMap;			// Albedo/BaseColor map
uniform sampler2D metallicRoughnessMap; // Metallic-Roughness map
uniform sampler2D normalMap;			// Normal map
uniform sampler2D emissiveMap;        // Emissive map (optional)


void main() {

 vec4 baseColor = texture(baseColorMap, TexCoords);

    FragColor = texture(baseColorMap, TexCoords); // Sample texture color at UV coordinates
}